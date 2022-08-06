#ifndef _EVHTTP_SERVER_H_
#define _EVHTTP_SERVER_H_

#include <event2/http.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <regex>
#include "ThreadPool.h"

/**
 * @def   USE_LINUX_REGEX_API
 * @brief Since some compilers are not good at implementing regular expressions in C++11,
 * Under linux, choose to use the system api to support path regular matching,
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
 * evhttp_cmd_type is an enumerated type in libevent. 
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
 * @brief EVHttpServer is just an http server implemented by encapsulating
 * libevent using c++, It provides:
 * - Simpler api
 * - Use thread pool to handle http requests
 * - Support regular matching path
 */
class EVHttpServer
{
public:
    /**
     * @brief HttpKeyVal is a key-value pair used to 
     * represent the HTTP head and HTTP Query
     */
    struct HttpKeyVal
    {
        std::string key;
        std::string value;
    };

    /**
     * @brief HttpReq is an encapsulation class for http requests, which
     * provides access to method, path, body, etc. 
     *   It is not allowed to be created externally.
     *   Here is an example illustrating the parts of the url:
     *         ┌──useriunfo─┐ ┌────host─────┐ ┌port┐ 
     *   https:admin:password@www.example.com:123456/forum/questions/?tag=networking&order=newest#top
     *└scheme┘ └────────────authority──────────────┘└────path───────┘ └────────query────────────┘ └fragment┘
     *   Some components of the uri do not provide access functions.
     */
    class HttpReq
    {
    public:
        evhttp_cmd_type method() const;
        std::string methodStr() const;
        std::string path() const;
        std::string body() const;
        std::string uri() const;
        std::string host() const;

        void headers(std::list<HttpKeyVal> & ret) const;
        bool findHeader(const std::string & key, std::string & value) const;

        void querys(std::list<HttpKeyVal> & ret) const;
        bool findQuery(const std::string & key, std::string & value) const;
    private:
        HttpReq(const HttpReq &) = delete;
        HttpReq & operator = (const HttpReq &) = delete;
        HttpReq(evhttp_request * req);
        struct evhttp_request * m_request = nullptr;
        friend EVHttpServer;
    };

    /**
     * @brief HttpRes is the encapsulation class of http response, which
     * provides methods for setting body, head, code, etc.
     * It is not allowed to be created externally.
     */
    class HttpRes
    {
    public:
        bool setBody(const std::string & body);
        bool addHeader(const HttpKeyVal & header);
        bool addHeaders(const std::list<HttpKeyVal> & list);
        bool setHeaders(const std::list<HttpKeyVal> & list);

        void setCode(const int & code);
        void setReason(const std::string & reason);
    private:
        HttpRes(const HttpRes &) = delete;
        HttpRes & operator = (const HttpRes &) = delete;
        HttpRes(evhttp_request * req);
    private:
        int getCode(void) const;
        const char * getReason(void) const;
    private:
        int m_code = HTTP_OK;
        std::string m_reason;
        struct evhttp_request * m_request = nullptr;
        bool m_initBody = false;
        friend EVHttpServer;
    };

    using ReqHandler = void (*)(const HttpReq & req, HttpRes & res, void * arg);/* Define http request callback function type */
public:
    /**
     * @brief Path and method are used to represent an HTTP request 
     */
    struct PathAndMethod
    {
        evhttp_cmd_type method;
        std::string path;
    public:
        PathAndMethod() : method(EVHTTP_REQ_GET)
        {
        }
        PathAndMethod(evhttp_cmd_type m, const std::string & u) : method(m), path(u)
        {
        }
        bool operator == (const PathAndMethod & info) const
        {
            return ((this->method == info.method) && (this->path == info.path));
        }
    };
public:
    EVHttpServer();
    virtual ~EVHttpServer();
    bool init(const unsigned int port, const std::string & ip = "0.0.0.0");
    bool init(const std::list<unsigned int> & portList, const std::string & ip = "0.0.0.0");
    bool start(const unsigned int threadNum = 5);
    bool stop();
    bool addHandler(const PathAndMethod & reqArg, const ReqHandler & handler, void * arg = nullptr);
    bool rmHandler(const PathAndMethod & reqArg);
    bool addRegHandler(const PathAndMethod & reqArg, const ReqHandler & handler, void * arg = nullptr);
    bool rmRegHandler(const PathAndMethod & reqArg);
private:
    bool deInit();
    static void handleHttpEvent(struct evhttp_request * request, void * arg);
    static void * dispatchThread(void * arg);
private:
    struct PathAndMethodHash
    {
        std::size_t operator () (const PathAndMethod & t) const
        {
            std::string key = std::to_string(t.method) + t.path;
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
        PathAndMethod reqArg;
#if USE_LINUX_REGEX_API
        regex_t reg;
#else
        std::regex reg;
#endif
        CallBackBind callbackBind;
    };
private:
    static void dealTask(struct evhttp_request * request, const PathAndMethod & reqArg, const CallBackBind & handleBind);
private:
    std::thread * m_thread = nullptr;       /* dispatch thread */
    volatile bool m_isInited = false;       /* initialized flag */
    volatile bool m_isRunning = false;      /* dispatch thread run flag */
    event_base * m_base = nullptr;
    std::list<void *> m_evhttpList;
    std::mutex m_mutex;
    std::unordered_map<PathAndMethod, CallBackBind, PathAndMethodHash> m_handlerMap;  /* map of request and callback functions */
    std::list<RegNode> m_regList;
    ThreadPool * m_threadPool = nullptr;
    EVHttpServer(const EVHttpServer &) = delete;
    EVHttpServer & operator = (const EVHttpServer &) = delete;
};

#endif
