#ifndef _EVHTTP_SERVER_H_
#define _EVHTTP_SERVER_H_

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>

#include <string>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <regex>

#ifndef HTTP_SERVER_USE_THREADPOOL
    #define HTTP_SERVER_USE_THREADPOOL (1) /* Whether to use thread pool, configurable */
#endif

#if HTTP_SERVER_USE_THREADPOOL
    #include "ThreadPool.h"
#endif

/**
 * @def   USE_LINUX_REGEX_API
 * @brief Since some compilers are not good at implementing regular expressions in C++11,
 * Under linux, choose to use the system api to support url regular matching,
 * There is also an option to use the regex support in C++11.
 */
#define USE_LINUX_REGEX_API (1)

#ifdef _WIN32
    #define USE_LINUX_REGEX_API (0)
#else
    #include <sys/types.h>
    #include <regex.h>
#endif

/*
 * enum evhttp_cmd_type {
 *    EVHTTP_REQ_GET     = 1 << 0,
 *    EVHTTP_REQ_POST    = 1 << 1,
 *    EVHTTP_REQ_HEAD    = 1 << 2,
 *    EVHTTP_REQ_PUT     = 1 << 3,
 *    EVHTTP_REQ_DELETE  = 1 << 4,
 *    EVHTTP_REQ_OPTIONS = 1 << 5,
 *    EVHTTP_REQ_TRACE   = 1 << 6,
 *    EVHTTP_REQ_CONNECT = 1 << 7,
 *    EVHTTP_REQ_PATCH   = 1 << 8
 *};
 */

/**
 * @brief EVHttpServer is an http server implemented by encapsulating libevent
 * using c++11, It provides:
 * - Simpler api
 * - Use thread pool to handle http requests
 * - Support regular matching url
 */
class EVHttpServer
{
public:
    struct HttpKeyVal
    {
        std::string key;
        std::string value;
    };

    /**
     * @brief HttpReq is an encapsulation class for http requests, which
     * provides access to methods and urls
     * body and other methods. It is not allowed to be created externally.
     */
    class HttpReq
    {
    public:
        evhttp_cmd_type method() const;
        std::string methodStr() const;
        std::string url() const;
        std::string body() const;

        void headers(std::list<HttpKeyVal> & ret) const;
        bool findHeader(const std::string & key, std::string & value) const;

        void querys(std::list<HttpKeyVal> & ret) const;
        bool findQuery(const std::string & key, std::string & value) const;
    private:
        HttpReq(const HttpReq &) = delete;
        HttpReq & operator = (const HttpReq &) = delete;
        friend EVHttpServer;
        HttpReq(evhttp_request * req);
        struct evhttp_request * m_request = nullptr;
    };

    /**
     * @brief HttpRes is the encapsulation class of http response, which
     * provides methods for setting body, head, code, etc.
     * It is not allowed to be created externally.
     */
    class HttpRes
    {
        HttpRes(evhttp_request * req);
        struct evhttp_request * m_request = nullptr;
        bool m_initBody = false;
    public:
        bool setBody(const std::string & body);
        bool addHeader(const HttpKeyVal & header);
        bool addHeaders(std::list<HttpKeyVal> & list);
        bool setHeaders(std::list<HttpKeyVal> & list);

        void setCode(const int & code);
        void setReason(const std::string & reason);
    private:
        HttpRes(const HttpRes &) = delete;
        HttpRes & operator = (const HttpRes &) = delete;
    private:
        int getCode(void) const;
        const char * getReason(void) const;
    private:
        int m_code = HTTP_OK;
        std::string m_reason;
        friend EVHttpServer;
    };

    using ReqHandler = bool (*)(const HttpReq & req, HttpRes & res, void * arg);/* Define http request callback function type */
public:
    struct UrlAndMethod
    {
        evhttp_cmd_type method;
        std::string url;
    public:
        UrlAndMethod() : method(EVHTTP_REQ_GET)
        {
        }
        UrlAndMethod(evhttp_cmd_type m, const std::string & u) : method(m), url(u)
        {
        }
        bool operator == (const UrlAndMethod & info) const
        {
            return ((this->method == info.method) && (this->url == info.url));
        }
    };
public:
    EVHttpServer();
    virtual ~EVHttpServer();
    bool init(const unsigned int port, const std::string & ip = "0.0.0.0");
    bool start(unsigned int threadNum = 1);
    bool stop();
    bool addHandler(const UrlAndMethod & reqArg, const ReqHandler & handler, void * arg = nullptr);
    bool rmHandler(const UrlAndMethod & reqArg);
    bool addRegHandler(const UrlAndMethod & reqArg, const ReqHandler & handler, void * arg = nullptr);
    bool rmRegHandler(const UrlAndMethod & reqArg);
private:
    bool deInit();
    static void handleHttpEvent(struct evhttp_request * request, void * arg);
    static void * dispatchThread(void * arg);
private:
    struct UrlAndMethodHash
    {
        std::size_t operator () (const UrlAndMethod & t) const
        {
            std::string key = std::to_string(t.method) + t.url;
            std::hash<std::string> strHash;
            return strHash(key);
        }
    };
    struct CallBackBind
    {
        ReqHandler func;
        void * arg;
    };
    struct RegNode
    {
        UrlAndMethod reqArg;
#if USE_LINUX_REGEX_API
        regex_t reg;
#else
        std::regex reg;
#endif
        CallBackBind callbackBind;
    };
private:
    static void dealTask(struct evhttp_request * request, const UrlAndMethod & reqArg, const CallBackBind & handleBind);
private:
    std::thread * m_thread = nullptr;       /* dispatch thread */
    volatile bool m_isInited = false;       /* initialized flag */
    volatile bool m_isRunning = false;      /* dispatch thread run flag */
    event_base * m_base = nullptr;
    evhttp * m_eventHttp = nullptr;
    std::mutex m_mutex;
    std::unordered_map<UrlAndMethod, CallBackBind, UrlAndMethodHash> m_handlerMap;  /* map of request and callback functions */
    std::list<RegNode> m_regList;

#if HTTP_SERVER_USE_THREADPOOL
    ThreadPool * m_threadPool = nullptr;
#endif
    EVHttpServer(const EVHttpServer &) = delete;
    EVHttpServer & operator = (const EVHttpServer &) = delete;
};

#endif
