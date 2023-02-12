#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
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

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
