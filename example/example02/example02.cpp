#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib,"ws2_32.lib")
#endif

EVHttpServer g_server;
static volatile bool g_running = true; /* run flag */

/**
 * @brief      Ctrl+C signal processing function, used to modify the running flag to exit the program
 * @param[in]
 * @param[out]
 * @retval
 */
void sighandler(int signum)
{
    if(SIGINT == signum)
    {
        std::cout << "Recvive SIGINT siganl, will exit." << std::endl;
        g_running = false;
    }
}

/**
 * @brief      http request callback function
 * @param[in]
 * @param[out]
 * @retval
 */
void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::list<EVHttpServer::HttpKeyVal> inList;
    std::cout << "Thread id:" << std::this_thread::get_id() << std::endl;

    /* http request line content */
    std::cout << req.methodStr() << " " << req.path() << std::endl;

    /* http request headers */
    req.headers(inList);
    for(auto iter = inList.begin(); iter != inList.end(); ++iter)
    {
        std::cout << iter->key << ":" << iter->value << std::endl;
    }
    std::cout << std::endl;

    /* http request parameters */
    req.queries(inList);
    for(auto iter = inList.begin(); iter != inList.end(); ++iter)
    {
        std::cout << iter->key << ":" << iter->value << std::endl;
    }
    std::cout << std::endl;

    /* http request body */
    std::cout << req.body() << std::endl;

    /* custom parameters passed in when the user registers the callback function */
    std::cout << "arg : " << (const char *)arg << std::endl;
    std::cout << std::endl;

    /* set http response headers */
    std::list<EVHttpServer::HttpKeyVal> outlist;
    outlist.push_back({"Server", "Apache"});
    outlist.push_back({"Content-Type", "application/json;charset=UTF-8"});
    res.addHeaders(outlist);

    /* set http response body */
    std::string body = R"({"status":"OK", "msg":""})";
    res.setBody(body);

    /* set the http reply code and reason. If not set, use the default value of 200 OK */
    res.setCode(200);
    res.setReason("OK");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

/**
 * @brief      When this callback is executed, the registered callback function will be removed
 * @param[in]
 * @param[out]
 * @retval
 */
void handleFunRemoveTest(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::cout << "handleFunRemoveTest" << std::endl;

    EVHttpServer::PathAndMethod reqArg;
    reqArg.method = EVHttpServer::REQ_POST;
    reqArg.path = "/api/removeAfterhandle";
    g_server.rmHandler(reqArg);
}

int main(int argc, const char *argv[])
{
    bool ret = true;

#ifdef _WIN32
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        wVersionRequested = MAKEWORD(2, 2);
        WSAStartup(wVersionRequested, &wsaData);
    }
#endif

    const char *str = "user string arg";
    int port = 9999;

    ret = ret && g_server.addHandler({EVHttpServer::REQ_POST, "/api/fun1"}, handleFunc, (void*)str);
    ret = ret && g_server.addHandler({EVHttpServer::REQ_POST, "/api/removeAfterhandle"}, handleFunRemoveTest, (void*)str);

    /* path regular matching test */
    ret = ret && g_server.addRegHandler({EVHttpServer::REQ_POST, "/api/fun[1-9]+"}, handleFunc, (void*)str);

    ret = ret && g_server.init(port);
    ret = ret && g_server.start(5); /* Start and run 5 event processing threads */
    if (ret)
    {
        std::cout << "start http server at port:" << port << std::endl;
    }
    else
    {
        std::cout << "start http server fail" << std::endl;
        goto EXIT;
    }

    /* capture Ctrl+C signal */
    if (signal(SIGINT, sighandler) == SIG_ERR)
    {
        std::cout << "Fail to signal" << std::endl;
        goto EXIT;
    }

    while(g_running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

EXIT:

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
