﻿#include "EVHttpServer.h"
#include <cstring>
#include <signal.h>
#include "event2/http.h"
#include "event2/thread.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/keyvalq_struct.h"
#include "event2/buffer.h"
#include "ThreadPool.h"

#define EVLOG_FATAL(errnum, fmt, ...) printf("[F] %s:%d [%s] errnum=%d " fmt "\n", __FILE__, __LINE__, __FUNCTION__, errnum, ##__VA_ARGS__)
#define EVLOG_ERROR(errnum, fmt, ...) printf("[E] %s:%d [%s] errnum=%d " fmt "\n", __FILE__, __LINE__, __FUNCTION__, errnum, ##__VA_ARGS__)
#define EVLOG_WARN(errnum, fmt, ...)  printf("[W] %s:%d [%s] errnum=%d " fmt "\n", __FILE__, __LINE__, __FUNCTION__, errnum, ##__VA_ARGS__)
#define EVLOG_INFO(fmt, ...)          printf("[I] %s:%d [%s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define EVLOG_DEBUG(fmt, ...)         printf("[D] %s:%d [%s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define EVLOG_TRACE(fmt, ...)         printf("[T] %s:%d [%s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

/**
 * @brief     Construct EVHttpServer object
 */
EVHttpServer::EVHttpServer()
{
}

/**
 * @brief      initialize http server
 * @param[in]  port : http port to be listen
 * @param[in]  ip IP address, default parameter "0.0.0.0"
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::init(const unsigned int port, const std::string & ip)
{
    std::list<unsigned int> list;
    list.push_back(port);

    return init(list, ip);
}

/**
 * @brief      Initialize the http server with multiple ports.
 * @param[in]  portList : http port list
 * @param[in]  ip IP address, default parameter "0.0.0.0"
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::init(const std::list<unsigned int> & portList, const std::string & ip)
{
    bool ret = true;
    std::lock_guard<std::mutex> locker(m_mutex);

    if(m_isInited)
    {
        EVLOG_ERROR(-1, "Already inited, can not init again.");
        return false;
    }

    do
    {
#if !(defined _WIN32 )
        /*
         *   If one end of the socket closes the connection, while the other end
         * still writes data to it, it will receive an RST response after the
         * first write of data.
         *   After writing data again, the kernel will send a SIGPIPE signal to
         * the process to notify the process that the connection has been disconnected.
         *   The default processing of the SIGPIPE signal is to terminate the program,
         * so here set ignores the SIGPIPE signal.
         */
        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) 
        {
            EVLOG_ERROR(-1, "Fail to signal.");
            ret = false;
            break;
        }
#endif

        /*
         * In a multi-threaded environment, if the evthread_use_windows_threads
         * or evthread_use_pthreads functions are not called, the
         * event_base_dispatch function will block all the time. Even if the
         * event_base_loopbreak or event_base_loopexit is called, 
         * the event_base_dispatch cannot exit the event loop.
         */
#ifdef _WIN32
        evthread_use_windows_threads();
#else
        evthread_use_pthreads();
#endif

        m_base = event_base_new();
        if(nullptr == m_base)
        {
            ret = false;
            break;
        }

        for(const auto & port : portList)
        {
            evhttp * pEvhttp = evhttp_new(m_base);
            if(nullptr == pEvhttp)
            {
                ret = false;
                break;
            }

            if(0 != evhttp_bind_socket(pEvhttp, ip.c_str(), port))
            {
                EVLOG_ERROR(-1, "evhttp_bind_socket failed, %s:%u", ip.c_str(), port);
                ret = false;
                break;
            }
            evhttp_set_allowed_methods(pEvhttp, EVHTTP_REQ_GET | EVHTTP_REQ_POST | EVHTTP_REQ_HEAD | EVHTTP_REQ_PUT |
                    EVHTTP_REQ_DELETE | EVHTTP_REQ_OPTIONS | EVHTTP_REQ_TRACE | EVHTTP_REQ_CONNECT | EVHTTP_REQ_PATCH);

            evhttp_set_gencb(pEvhttp, handleHttpEvent, this);
            m_evhttpList.push_back(static_cast<void *>(pEvhttp));
        }

        /* set initialized flag */
        m_isInited = true;

    }while(0);

    if(!ret)
    {
        for(auto & pEvhttp : m_evhttpList)
        {
            evhttp_free(static_cast<evhttp *>(pEvhttp));
            pEvhttp = nullptr;
        }
        m_evhttpList.clear();

        if(m_base)
        {
            event_base_free(m_base);
            m_base = nullptr;
        }
    }

    return ret;
}

/**
 * @brief      Deinitialize, release resources
 * @retval     true : success 
 * @retval     false : failed
 */
bool EVHttpServer::deInit()
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if(m_isRunning)
    {
        EVLOG_ERROR(-1, "Uninit fail, do stop before deInit.");
        return false;
    }

    if(false == m_isInited)
    {
        return true;
    }

    for(auto & pEvhttp : m_evhttpList)
    {
        evhttp_free(static_cast<evhttp *>(pEvhttp));
        pEvhttp = nullptr;
    }
    m_evhttpList.clear();

    if(m_base)
    {
        event_base_free(m_base);
        m_base = nullptr;
    }

    m_isInited = false;
    return true;
}

/**
 * @brief      Start http server，run event dispatch thread. if threadNum > 0,
 * the thread pool will be created and start threadNum threads.
 * @param[in]  threadNum : When using ThreadPool, specify thenumber of threads in the thread pool
 * @retval     true : success 
 * @retval     false : failed
 */
bool EVHttpServer::start(const unsigned int threadNum)
{
    bool ret = true;
    std::lock_guard<std::mutex> locker(m_mutex);

    if(!m_isInited)
    {
        EVLOG_ERROR(-1, "Start fail, no init.");
        return false;
    }

    if(m_isRunning)
    {
        EVLOG_ERROR(-1, "Start fail, already started.");
        return false;
    }

    do
    {
        /* Create thread pool */
        if(threadNum > 0)
        {
            m_threadPool = new(std::nothrow) ThreadPool(threadNum);
            if(nullptr == m_threadPool)
            {
                EVLOG_ERROR(-1, "Create thread pool failed!");
                ret = false;
                break;
            }
            EVLOG_INFO("ThreadPool create thread num:%u", threadNum);
        }
        else
        {
            EVLOG_WARN(0, "Parameter threadNum:%u, not use thread pool.", threadNum);
        }

        /* Create dispatch thread */
        m_isRunning = true;

        m_thread = new(std::nothrow) std::thread(&EVHttpServer::dispatchThread, this);
        if(nullptr == m_thread)
        {
            m_isRunning = false;
            EVLOG_ERROR(-1, "Dispatch thread create failed!");
            ret = false;
            break;
        }
    }while(0);

    if(!ret)
    {
        if(m_thread)
        {
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }

        if(m_threadPool)
        {
            delete m_threadPool;
            m_threadPool = nullptr;
        }
    }

    return m_isRunning;
}

/**
 * @brief      Register the callback handler function corresponding to the http request, 
 * handlers can be registered both before and after the start function is called.
 * @param[in]  reqArg : http request parameters, including request method and path, see @ref PathAndMethod for details 
 * @param[in]  handler : callback handler function
 * @param[in]  arg : User-defined parameters
 * @retval     true : Add success
 * @retval     false : Add failed
 */
bool EVHttpServer::addHandler(const PathAndMethod & reqArg, const ReqHandler & handler, void * arg)
{
    bool ret = true;

    if(nullptr == handler)
    {
        EVLOG_ERROR(-1, "Add handler fail, handler is null.");
        return false;
    }

    std::lock_guard<std::mutex> locker(m_mutex);

    if(m_handlerMap.find(reqArg) != m_handlerMap.end())
    {
        ret = false;
    }
    else
    {
        CallBackBind  callBackBind = {handler, arg};
        m_handlerMap.insert(std::make_pair(reqArg, callBackBind));
    }

    return ret;
}

/**
 * @brief      Remove the callback handler corresponding to the http request
 * @param[in]  reqArg : http request parameters, including request method and path, see @ref PathAndMethod for details 
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::rmHandler(const PathAndMethod & reqArg)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    auto iter = m_handlerMap.find(reqArg);
    if (iter != m_handlerMap.end())
    {
        m_handlerMap.erase(iter);
        return true;
    }

    return false;
}

/**
 * @brief      task handler function
 * @param[in]  request : A pointer representing the type of http request in libevent
 * @param[in]  reqArg : http request parameters, including request method and path, see @ref PathAndMethod for details 
 * @param[in]  handleBind : Callback parameters, including callback function and user parameters
 * @return     void
 */
void EVHttpServer::dealTask(struct evhttp_request * request, const PathAndMethod & reqArg, const CallBackBind & handleBind)
{
    HttpReq req(request);
    HttpRes res(request);

    handleBind.func(req, res, handleBind.arg);

    evbuffer * outputBuf = evhttp_request_get_output_buffer(request);
    evhttp_send_reply(request, res.getCode(), res.getReason(), outputBuf);
}

/**
 * @brief
 *   Handle http request event.
 *   Find the processing function according to the request parameters.
 * The request parameters will first match the parameters in the map, and then
 * match the items in the regex list if the matching fails.
 *   If thread pool in used, the task will be push in thread pool,
 * otherwise task will be handle immediately.
 * @param[in]  request : a request object of livevent
 * @param[in]  arg : User defined parameter. use "this" pointer here.
 * @return     void
 */
void EVHttpServer::handleHttpEvent(struct evhttp_request * request, void * arg)
{
    PathAndMethod reqArg;
    EVHttpServer * pThis = static_cast<EVHttpServer *>(arg);

    const struct evhttp_uri * pUri = evhttp_request_get_evhttp_uri(request);
    const char * path = evhttp_uri_get_path(pUri);
    reqArg.path = (path != nullptr) ? path : "";
    reqArg.method = static_cast<ReqType>(evhttp_request_get_command(request));

    pThis->m_mutex.lock();

    auto it = pThis->m_handlerMap.find(reqArg);
    if (it != pThis->m_handlerMap.end())
    {
        CallBackBind handleBind = it->second;

        /* Unlock before executing callback,Prevent deadlock caused by calling
         * the public function that locks m_mutex in the callback function
         */
        pThis->m_mutex.unlock();

        if(pThis->m_threadPool)
        {
            pThis->m_threadPool->enqueue(dealTask, request, reqArg, handleBind);
        }
        else
        {
            dealTask(request, reqArg, handleBind);
        }
    }
    else
    {
        for(auto iter = pThis->m_regList.begin(); iter != pThis->m_regList.end(); ++iter)
        {
    #if (!USE_LINUX_REGEX_API)
            if (false == std::regex_search(reqArg.path.c_str(), iter->reg))
            {
                continue;
            }
    #else
            /* The effect of regexec is similar to regex_search, not regex_match */
            int regRet = regexec(&iter->reg, reqArg.path.c_str(), 0, nullptr, 0);
            if((0 != regRet) || ( iter->reqArg.method != reqArg.method)) 
            {
                continue;
            }
    #endif
            CallBackBind handleBind = iter->callbackBind;

            /* Unlock before executing callback,Prevent deadlock caused by calling
             * the public function that locks m_mutex in the callback function
             */
            pThis->m_mutex.unlock();

            if(pThis->m_threadPool)
            {
                pThis->m_threadPool->enqueue(dealTask, request, reqArg, handleBind);
            }
            else
            {
                dealTask(request, reqArg, handleBind);
            }
            return;
        }
        pThis->m_mutex.unlock();
        evhttp_send_reply(request, HTTP_NOTFOUND, "Not found", nullptr);
    }
}

/**
 * @brief      Stop the event dispatch thread and destroy the thread pool.
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::stop()
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if(m_isRunning)
    {
        m_isRunning = false;
        /*
         * See http://www.wangafu.net/~nickm/libevent-book/Ref3_eventloop.html
         * Note also that event_base_loopexit(base,NULL) and
         * event_base_loopbreak(base) act differently when no event loop is
         * running: loopexit schedules the next instance of the event loop to
         * stop right after the next round of callbacks are run (as if it had
         * been invoked with EVLOOP_ONCE) whereas loopbreak only stops a
         * currently running loop, and has no effect if the event loop isn’t
         * running.
         */
        event_base_loopbreak(m_base);
        event_base_loopexit(m_base, nullptr);

        if(m_thread)
        {
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }

        if(m_threadPool)
        {
            delete m_threadPool;
            m_threadPool = nullptr;
        }
    }

    return (false == m_isRunning);
}

/**
 * @brief      Event dispatch thread function
 * @return     void
 */
void EVHttpServer::dispatchThread()
{
    if(m_base)
    {
        event_base_dispatch(m_base);
    }
}

/**
 * @brief      The destructor of EVHttpServer, the stop and deInit functions
 * will be executed in this function, and other resources will be released.
 */
EVHttpServer::~EVHttpServer()
{
    stop();
    deInit();

#if USE_LINUX_REGEX_API
    std::lock_guard<std::mutex> locker(m_mutex);
    for(auto iter = m_regList.begin(); iter != m_regList.end(); ++iter)
    {
        regfree(&iter->reg);
    }
#endif

}

/**
 * @brief      Add a handler for "regex path", handlers can be registered both before and after the start function is called.
 * @param[in]  reqArg : http request parameters, including request method and path, see @ref PathAndMethod for details 
 * @param[in]  handler : callback function
 * @param[in]  arg : User-defined parameters
 * @retval     true : Add success
 * @retval     false : Add failed
 */
bool EVHttpServer::addRegHandler(const PathAndMethod & reqArg, const ReqHandler & handler, void * arg)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if(nullptr == handler)
    {
        EVLOG_ERROR(-1, "Add regex handler fail, handler is null.");
        return false;
    }

    /*
     * Traverse the linked list, if found to the same method and path description
     * has been inserted before, then failed to insert.
     */
    for(auto iter = m_regList.begin(); iter != m_regList.end(); ++iter)
    {
        if(iter->reqArg == reqArg)
        {
            return false;
        }
    }

    /*
     * Insert new node into linked list
     */
#if (!USE_LINUX_REGEX_API)
    std::regex reg(reqArg.path);
#else
    int ret = 0;
    regex_t reg = {0};

    /*
     * REG_EXTENDED
     *   Use POSIX Extended Regular Expression syntax when interpreting  regex.
     * REG_NOSUB
     *   Do not report position of matches.  The nmatch and pmatch arguments to
     *   regexec() are ignored if the pattern buffer supplied was compiled with
     *   this flag set.
     */
    ret = regcomp(&reg, reqArg.path.c_str(), REG_EXTENDED | REG_NOSUB);
    if(0 != ret)
    {
        char errBuf[256] = {0};
        regerror(ret, &reg, errBuf, sizeof(errBuf));
        EVLOG_ERROR(-1, "Fail to regcomp:%s", errBuf);
        return false;
    }
#endif
    m_regList.push_back(RegNode{reqArg, reg, {handler, arg}});

    return true;
}

/**
 * @brief      Remove handlers for "regex path"
 * @param[in]  reqArg : http request parameters, including request method and path, see @ref PathAndMethod for details 
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::rmRegHandler(const PathAndMethod & reqArg)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    auto iter = m_regList.begin();
    for(; iter != m_regList.end(); ++iter)
    {
        if(iter->reqArg == reqArg)
        {
            break;
        }
    }

    if(iter != m_regList.end())
    {
#if USE_LINUX_REGEX_API
        regfree(&iter->reg);
#endif
        m_regList.erase(iter);
        return true;
    }

    return false;
}

/**
 * @brief     Constructor of HttpReq, which is a private 
 * function and is not allowed to be called externally
 */
EVHttpServer::HttpReq::HttpReq(evhttp_request * req) : m_request(req)
{
}

/**
 * @brief      Get http request method
 * @return     The return value is the enumeration type of ReqType
 */
EVHttpServer::ReqType EVHttpServer::HttpReq::method() const
{
    return static_cast<ReqType>(evhttp_request_get_command(m_request));
}

/**
 * @brief      Get http request host 
 * @return     Returns the host associated with the request. If a client sends
 * an absolute URI, the host part of that is preferred. Otherwise, the input headers are
 * searched for a Host: header. NULL is returned if no absolute URI or Host:header is provided.
 */
std::string EVHttpServer::HttpReq::host() const
{
    return evhttp_request_get_host(m_request);
}

/**
 * @brief      Get http request method string
 * @return     The return value is after converting the ReqType(evhttp_cmd_type) enumeration 
 * type to a string, as listed below:
 * "GET","POST","HEAD","PUT","DELETE","OPTIONS","TRACE","CONNECT","PATCH"
 */
std::string EVHttpServer::HttpReq::methodStr() const
{
    std::string ret;

    switch (evhttp_request_get_command(m_request))
    {
    case  EVHTTP_REQ_GET:
        ret = "GET";
        break;
    case  EVHTTP_REQ_POST:
        ret = "POST";
        break;
    case  EVHTTP_REQ_HEAD:
        ret = "HEAD";
        break;
    case  EVHTTP_REQ_PUT:
        ret = "PUT";
        break;
    case  EVHTTP_REQ_DELETE:
        ret = "DELETE";
        break;
    case  EVHTTP_REQ_OPTIONS:
        ret = "OPTIONS";
        break;
    case  EVHTTP_REQ_TRACE:
        ret = "TRACE";
        break;
    case  EVHTTP_REQ_CONNECT:
        ret = "CONNECT";
        break;
    case  EVHTTP_REQ_PATCH:
        ret = "PATCH";
        break;
    default:
        break;
    }

    return ret;
}

/**
 * @brief      Get http request path
 * @return     path string
 */
std::string EVHttpServer::HttpReq::path() const
{
    const struct evhttp_uri * pUri = evhttp_request_get_evhttp_uri(m_request);
    const char * path = evhttp_uri_get_path(pUri);
    return (path != nullptr) ? path : "";
}

/**
 * @brief      Get http request uri
 * @param[in]  decode : If decode is equal to true, the decoded uri will be returned
 * @return     uri string
 */
std::string EVHttpServer::HttpReq::uri(bool decode) const
{
    const char * uri = evhttp_request_get_uri(m_request);
    if(!decode)
    {
        return (uri != nullptr) ? uri : "";
    }
    else
    {
        char * decodedUri = evhttp_uridecode(uri, 1, nullptr);
        if(decodedUri)
        {
            std::string ret = decodedUri;
            free(decodedUri);
            decodedUri = nullptr;
            return ret;
        }
        return "";
    }
}

/**
 * @brief      Get http request body
 * @return     body string
 */
std::string EVHttpServer::HttpReq::body() const
{
    evbuffer * inputBuf = evhttp_request_get_input_buffer(m_request);
    auto bufLen = evbuffer_get_length(inputBuf);

    /*
     *   In windows, the compiler requires the size of the array allocated in
     * the stack to be constant.
     *   In Linux, we choose to allocate small memory in the stack and
     * large memory in the heap.
     */
#ifndef _WIN32
    if(bufLen > 1024)
    {
#endif
        std::unique_ptr<char[]>bufIn(new(std::nothrow) char[bufLen]);/* no need memset */
        if(nullptr == bufIn.get())
        {
            return "";
        }
        evbuffer_copyout(inputBuf, bufIn.get(), bufLen);
        return std::string(bufIn.get(), bufLen);
#ifndef _WIN32
    }
    else
    {
        char bufIn[bufLen]; /* no need memset */
        evbuffer_copyout(inputBuf, bufIn, bufLen);
        return std::string(bufIn, bufLen);
    }
#endif

}

/**
 * @brief      Get http request body raw data
 * @return     std::vector which provides raw data of body
 */
std::vector<char> EVHttpServer::HttpReq::bodyRaw() const
{
    evbuffer * inputBuf = evhttp_request_get_input_buffer(m_request);
    auto bufLen = evbuffer_get_length(inputBuf);

    std::vector<char> rawBody(bufLen);
    evbuffer_copyout(inputBuf, &rawBody[0], bufLen);

    return rawBody;
}

/**
 * @brief      Get http request headers
 * @param[out] ret : A list for storing http request headers
 * @return     void
 */
void EVHttpServer::HttpReq::headers(std::list<HttpKeyVal> & ret) const
{
    ret.clear();

    struct evkeyvalq * headers = evhttp_request_get_input_headers(m_request);
    for(struct evkeyval * h = headers->tqh_first; h; h = h->next.tqe_next)
    {
        ret.push_back({h->key, h->value});
    }
}

/**
 * @brief      find header in http request
 * @param[in]  key : header name
 * @param[out] value : header value
 * @retval     true : find success
 * @retval     false : not found
 */
bool EVHttpServer::HttpReq::findHeader(const std::string & key, std::string & value) const
{
    struct evkeyvalq * headers = evhttp_request_get_input_headers(m_request);
    const char * pVal = evhttp_find_header(headers, key.c_str());
    if(pVal)
    {
        value = pVal;
    }

    return (pVal != nullptr);
}

/**
 * @brief      Get queries in http request, if queries are encoded, they will be automatically decoded
 * @param[out] ret : A list for storing http request queries
 * @return     void
 */
void EVHttpServer::HttpReq::queries(std::list<HttpKeyVal> & ret) const
{
    ret.clear();

    const char * uri = evhttp_request_get_uri(m_request);
    struct evhttp_uri * evhttpUri = evhttp_uri_parse(uri);
    const char * queries = evhttp_uri_get_query(evhttpUri);
    if(queries)
    {
        struct evkeyvalq headers = {0};
        if(0 == evhttp_parse_query_str(queries, &headers))
        {
            for(struct evkeyval * h = headers.tqh_first; h; h = h->next.tqe_next)
            {
                ret.push_back({h->key, h->value});
            }
            evhttp_clear_headers(&headers);
        }
    }

    if(evhttpUri)
    {
        evhttp_uri_free(evhttpUri);
        evhttpUri = nullptr;
    }
}

/**
 * @brief      find http query, if queries are encoded, they will be automatically decoded
 * @param[in]  key : query name
 * @param[out] value : query value
 * @retval     true : find success
 * @retval     false : not found
 */
bool EVHttpServer::HttpReq::findQuery(const std::string & key, std::string & value) const
{
    const char * uri = evhttp_request_get_uri(m_request);
    struct evhttp_uri * evhttpUri = evhttp_uri_parse(uri);
    const char * queries = evhttp_uri_get_query(evhttpUri);
    const char * pVal = nullptr;
    if(queries)
    {
        struct evkeyvalq headers = {0};
        if(0 == evhttp_parse_query_str(queries, &headers))
        {
            pVal = evhttp_find_header(&headers, key.c_str());
            if(pVal)
            {
                value = pVal;
            }
            evhttp_clear_headers(&headers);
        }
    }

    if(evhttpUri)
    {
        evhttp_uri_free(evhttpUri);
        evhttpUri = nullptr;
    }

    return (pVal != nullptr);
}

/**
 * @brief      Helper function to decode a URI-escaped string or HTTP parameter.
 * @param[in]  in  : string to be decoded
 * @param[out] out : decoded string
 * @retval     true : success
 * @retval     false : failed
 */
bool EVHttpServer::HttpReq::decode(const std::string & in, std::string & out)
{
    char * decodedUri = evhttp_uridecode(in.c_str(), 1, nullptr);
    bool ret = (decodedUri != nullptr);
    if(ret)
    {
        out = decodedUri;
        free(decodedUri);
        decodedUri = nullptr;
    }
    return ret;
}

/**
 * @brief      Constructor of HttpRes, which is a private function and is not
 * allowed to be called externally
 */
EVHttpServer::HttpRes::HttpRes(evhttp_request * req) : m_request(req)
{
}

/**
 * @brief      set http response body
 * @param[in]  body : response body (std::string)
 * @retval     true : set success
 * @retval     false : set failed
 */
bool EVHttpServer::HttpRes::setBody(const std::string & body)
{
    if(m_initBody)
    {
        return false;
    }

    evbuffer * outputBuf = evhttp_request_get_output_buffer(m_request);
    bool ret = (0 == evbuffer_add(outputBuf, body.c_str(), body.size()));
    if(ret)
    {
        m_initBody = true;
    }

    return ret;
}

/**
 * @brief      set http response body
 * @param[in]  body : response body (std::vector)
 * @retval     true : set success
 * @retval     false : set failed
 */
bool EVHttpServer::HttpRes::setBody(const std::vector<char> & body)
{
    if(m_initBody)
    {
        return false;
    }

    evbuffer * outputBuf = evhttp_request_get_output_buffer(m_request);
    bool ret = (0 == evbuffer_add(outputBuf, &body[0], body.size()));
    if(ret)
    {
        m_initBody = true;
    }

    return ret;
}

/**
 * @brief      set http response headers
 * @param[in]  list : header list
 * @retval     true : set success
 * @retval     false : set failed
 */
bool EVHttpServer::HttpRes::setHeaders(const std::list<HttpKeyVal> & list)
{
    evhttp_clear_headers(evhttp_request_get_output_headers(m_request));
    return addHeaders(list);
}

/**
 * @brief      Add the headers of the http reply. Unlike setHeaders, this function will not clear the previously set headers.
 * @param[in]  list : header list
 * @retval     true : set success
 * @retval     false : set failed
 */
bool EVHttpServer::HttpRes::addHeaders(const std::list<HttpKeyVal> & list)
{
    bool ret = true;

    struct evkeyvalq * headers = evhttp_request_get_output_headers(m_request);
    for(auto iter = list.begin(); iter != list.end(); ++iter)
    {
        int evRet = evhttp_add_header(headers, iter->key.c_str(), iter->value.c_str());
        ret = ret && (0 == evRet);
    }

    return ret;
}

/**
 * @brief      Add http reply header (single)
 * @param[in]  header : A header to reply to the http client
 * @retval     true : set success
 * @retval     false : set failed
 */
bool EVHttpServer::HttpRes::addHeader(const HttpKeyVal & header)
{
    struct evkeyvalq * headers = evhttp_request_get_output_headers(m_request);
    return (0 == evhttp_add_header(headers, header.key.c_str(), header.value.c_str()));
}

/**
 * @brief     Set the HTTP response code to send
 * @param[in] code : the HTTP response code to send
 * @return    void
 */
void EVHttpServer::HttpRes::setCode(const int & code)
{
    m_code = code;
}

/**
 * @brief     Get the HTTP response code to send
 * @return    the code
 */
int EVHttpServer::HttpRes::getCode(void) const
{
    return m_code;
}

/**
 * @brief     Set reason which is a brief message to send with the response code
 * @param[in] reason : a brief message to send with the response code
 * @return    void
 */
void EVHttpServer::HttpRes::setReason(const std::string & reason)
{
    m_reason = reason;
}

/**
 * @brief     Get reason which is a brief message to send with the response code
 * @return    if the reason string length > 0, Returns a pointer to the reason address,
 *            otherwise return nullptr. libevent will give a default reason
 *            string when set evhttp_send_reply function "reason" parameter as NULL.
 */
const char * EVHttpServer::HttpRes::getReason(void) const
{
    return m_reason.size() ? m_reason.c_str() : nullptr;
}
