#ifndef _EVHTTP_SERVER_H_
#define _EVHTTP_SERVER_H_

#include <string>
#include <unordered_map>
#include <set>
#include <mutex>
#include <thread>
#include <list>
#include <regex>
#include <vector>
#include <functional>

#ifdef _WIN32
    /*
     * The EVHTTPSVR_EXPORT_SYMBOL macro is defined in the Cmake file that
     * compiles the EVHttpServer library 
     */
    #ifdef EVHTTPSVR_EXPORT_SYMBOL 
        #define EVHTTPSVR_DLL_DECLARE    __declspec( dllexport )
    #else
        #define EVHTTPSVR_DLL_DECLARE    // __declspec( dllimport )  //In order to use both source code and library at the same time, dllimport is not used
    #endif
#else
    #define EVHTTPSVR_DLL_DECLARE
#endif

/**
 * @def   USE_LINUX_REGEX_API
 * @brief Some compilers are not good at implementing regular expressions in C++11,
 * Under linux, default choose to use the system api to support path regular matching,
 * There is also an option to use the regex support in C++11.
 * The value of this macro can only be modified before compiling the EVHttpServer source code.
 */
#define USE_LINUX_REGEX_API (1)

#ifdef _WIN32
    #undef USE_LINUX_REGEX_API
    #define USE_LINUX_REGEX_API (0)
#else
    #include <sys/types.h>
    #include <regex.h>
#endif

/**
 * @brief ThreadPool is a data type in ThreadPool.h, 
 * In order to minimize compilation dependencies between files, 
 * use declarations instead of include header files.
 */
class ThreadPool;

/**
 * @brief evhttp_request is a data type in libevent. 
 * In order to minimize compilation dependencies between files, 
 * use declarations instead of include header files.
 */
struct evhttp_request;

/**
 * @brief event_base is a data type in libevent.
 * In order to minimize compilation dependencies between files,
 * use declarations instead of include header files.
 */
struct event_base;

/**
 * @brief evws_connection is a data type in libevent.
 * In order to minimize compilation dependencies between files,
 * use declarations instead of include header files.
 */
struct evws_connection;

/**
 * @brief EVHttpServer is just an http server implemented by encapsulating
 * libevent using c++, It provides:
 * - Simpler api
 * - Use thread pool to handle http requests
 * - Support regular matching path
 */
class EVHTTPSVR_DLL_DECLARE EVHttpServer
{
public:
    /**
     * @brief ReqType is the enumeration type of the http request method, 
     * refer to the evhttp_cmd_type type definition in libevent.
     */
     enum ReqType {
        REQ_GET     = 1 << 0,
        REQ_POST    = 1 << 1,
        REQ_HEAD    = 1 << 2,
        REQ_PUT     = 1 << 3,
        REQ_DELETE  = 1 << 4,
        REQ_OPTIONS = 1 << 5,
        REQ_TRACE   = 1 << 6,
        REQ_CONNECT = 1 << 7,
        REQ_PATCH   = 1 << 8
    };

    /**
     * @brief HttpKeyVal is a key-value pair used to 
     * represent the HTTP head and HTTP Query
     */
    struct EVHTTPSVR_DLL_DECLARE HttpKeyVal
    {
        std::string key;
        std::string value;
    };

    /**
     * @brief HttpReq is an encapsulation class for http requests, which
     * provides access to method, path, body, etc. 
     *   It is not allowed to be created externally.
     *   Some components of the uri do not provide access functions.
     */
    class EVHTTPSVR_DLL_DECLARE HttpReq
    {
    public:
        ReqType method() const;
        std::string methodStr() const;
        std::string path() const;
        std::string body() const;
        std::vector<char> bodyRaw() const;
        std::string uri(bool decode = false) const;
        std::string host() const;

        void headers(std::list<HttpKeyVal> & ret) const;
        bool findHeader(const std::string & key, std::string & value) const;

        void queries(std::list<HttpKeyVal> & ret) const;
        bool findQuery(const std::string & key, std::string & value) const;
        static bool decode(const std::string & in, std::string & out);
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
    class EVHTTPSVR_DLL_DECLARE HttpRes
    {
    public:
        bool setBody(const std::string & body);
        bool setBody(const std::vector<char> & body);
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
        int m_code = 200;
        std::string m_reason;
        struct evhttp_request * m_request = nullptr;
        bool m_initBody = false;
        friend EVHttpServer;
    };

    using ReqHandler = std::function<void (const HttpReq & req, HttpRes & res, void * arg) >;/* Define http request callback function type */

    /**
     * @brief Websocket connect session.
     */
    class EVHTTPSVR_DLL_DECLARE WSSession
    {
    public:
        void send(const char * msg, std::size_t len);
        void send(const std::string & msg);
        void close();
        std::string getClientIP(){return m_IP;}
        int getClientPort(){return m_port;}
    private:
        WSSession(const WSSession &) = delete;
        WSSession & operator = (const WSSession &) = delete;
        WSSession(){};
        void setClientIP(const std::string & ip){m_IP = ip;}
        void setClientPort(int port){m_port = port;}
    private:
        struct evws_connection *m_evws = nullptr;
        void * m_arg = nullptr;
        std::string m_IP;
        int m_port;
        friend EVHttpServer;
    };
    using WSOnOpenHandler = std::function<void (WSSession * pSession, void * arg)>;
    using WSOnCloseHandler = std::function<void (WSSession * pSession, void * arg)>;
    using WSOnMsgHandler = std::function<void (WSSession * pSession, int type, const unsigned char *data, size_t len, void *arg)>;
    struct WSCallback
    {
        WSOnOpenHandler onOpen = nullptr;
        WSOnCloseHandler onClose = nullptr;
        WSOnMsgHandler onMsg = nullptr;
    };

public:
    /**
     * @brief Path and method are used to represent an HTTP request 
     */
    struct EVHTTPSVR_DLL_DECLARE PathAndMethod
    {
    public:
        PathAndMethod() : method(ReqType::REQ_GET)
        {
        }
        PathAndMethod(ReqType m, const std::string & u) : method(m), path(u)
        {
        }
        bool operator == (const PathAndMethod & info) const
        {
            return ((this->method == info.method) && (this->path == info.path));
        }
    public:
        ReqType method;
        std::string path;
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
public:
    bool addWSHandler(const std::string & path, const WSCallback & callback, void * arg = nullptr);
    bool rmWSHandler(const std::string & path);
    void sendWSBroadcast(const char * msg, size_t len);
private:
    bool deInit();
    static void handleHttpEvent(struct evhttp_request * request, void * arg);
    void dispatchThread();
private:
    struct PathAndMethodHash
    {
        std::size_t operator () (const PathAndMethod & t) const
        {
            std::string key = std::to_string(t.method) + t.path;
            return std::hash<std::string>() (key);
        }
    };
    struct CallBackBind
    {
        ReqHandler func;
        void * arg;
    };
    struct WSCallBackBind
    {
        WSCallback cb;
        void * arg;
        EVHttpServer * server;
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
    static void dealWSTask(struct evhttp_request * request, const std::string & path, WSCallBackBind * handleBind);
    static void onWebSocketCloseCallback(struct evws_connection *evws, void *arg);
    static void onWebSocketMsgCallback(struct evws_connection *evws, int type, const unsigned char *data, size_t len, void *arg);
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
private:
    std::mutex m_wsMutex;
    std::unordered_map<std::string, WSCallBackBind*> m_wsHandlerMap;
    std::set<WSSession *> m_wsSessions;
};

#endif
