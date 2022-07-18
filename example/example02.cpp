#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib,"ws2_32.lib")
#endif

EVHttpServer g_server;
static volatile bool g_running = true; /* 运行标志位 */

/**
 * @brief      Ctrl+C信号的处理函数，用于修改运行标志位退出程序
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
 * @brief      http 请求回调函数
 * @param[in]
 * @param[out]
 * @retval
 */
void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::list<EVHttpServer::HttpKeyVal> inList;
    std::cout << "Thread id:" << std::this_thread::get_id() << std::endl;

    /* 请求行内容 */
    std::cout << req.methodStr() << " " << req.path() << std::endl;

    /* 请求头 */
    req.headers(inList);
    for(auto iter = inList.begin(); iter != inList.end(); ++iter)
    {
        std::cout << iter->key << ":" << iter->value << std::endl;
    }
    std::cout << std::endl;

    /* 请求参数 */
    req.querys(inList);
    for(auto iter = inList.begin(); iter != inList.end(); ++iter)
    {
        std::cout << iter->key << ":" << iter->value << std::endl;
    }
    std::cout << std::endl;

    /* 请求数据 */
    std::cout << req.body() << std::endl;

    /* 用户注册的回调函数参数 */
    std::cout << "arg : " << (const char *)arg << std::endl;
    std::cout << std::endl;

    /* 设置回复头 */
    std::list<EVHttpServer::HttpKeyVal> outlist;
    outlist.push_back({"Server", "Apache"});
    outlist.push_back({"Content-Type", "application/json;charset=UTF-8"});
    res.addHeaders(outlist);

    /* 设置回复body */
    std::string body = R"({"status":"OK", "msg":""})";
    res.setBody(body);

    /* 设置回复码和原因，可不设置使用默认值200 OK */
    res.setCode(200);
    res.setReason("OK");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

/**
 * @brief      这个回调被执行时会移除注册的回调函数
 * @param[in]
 * @param[out]
 * @retval
 */
void handleFunRemoveTest(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::cout << "handleFunRemoveTest" << std::endl;

    EVHttpServer::PathAndMethod reqArg;
    reqArg.method = EVHTTP_REQ_POST;
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

    ret = ret && g_server.addHandler({EVHTTP_REQ_POST, "/api/fun1"}, handleFunc, (void*)str);
    ret = ret && g_server.addHandler({EVHTTP_REQ_POST, "/api/removeAfterhandle"}, handleFunRemoveTest, (void*)str);

    /* path 正则匹配测试 */
    ret = ret && g_server.addRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}, handleFunc, (void*)str);

    ret = ret && g_server.init(port);
    ret = ret && g_server.start(5); /* 启动并运行5个事件处理线程 */
    if (ret)
    {
        std::cout << "start http server at port:" << port << std::endl;
    }
    else
    {
        std::cout << "start http server fail" << std::endl;
        goto EXIT;
    }

    /* 捕获Ctrl+C信号 */
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
