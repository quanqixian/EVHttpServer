#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <condition_variable>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

void lambdaTest01()
{
    volatile bool flag = false;
    std::mutex mtx;
    std::unique_lock<std::mutex> locker(mtx);
    std::condition_variable conditionVal;

    EVHttpServer server;
    server.init(9999, "0.0.0.0");
    server.addHandler({EVHttpServer::REQ_PUT, "/api/fun"},  [&](const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)->void {
        flag = true;
        conditionVal.notify_one();
    });
    server.start(5);

    conditionVal.wait_for(locker, std::chrono::seconds(10), [&]{return flag;});

    if(flag)
    {
        std::cout << "Recv" << std::endl;
    }
    else
    {
        std::cout << "No recv." << std::endl;
    }
}

void lambdaTest02()
{
    EVHttpServer server;
    server.init(9999);
    server.start();

    server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, [](const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)->void {
            std::cout << req.methodStr() << " " << req.path() << std::endl;
            std::cout << req.body() << std::endl;

            res.setBody(R"({"status":"OK"})");
            });

    signal(SIGINT, sighandler);

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main(int argc, const char *argv[])
{
#ifdef _WIN32
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
}
#endif

    lambdaTest01();
    lambdaTest02();

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
