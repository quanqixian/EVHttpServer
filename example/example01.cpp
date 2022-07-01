#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

bool handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::cout << req.methodStr() << " " << req.url() << std::endl;
    std::cout << req.body() << std::endl;

    res.setBody(R"({"status":"OK"})");
    res.setCode(200);

    return true;
}

int main(int argc, const char *argv[])
{
    EVHttpServer server;

    server.addHandler({EVHTTP_REQ_POST, "/api/fun"}, handleFunc, nullptr);
    server.init(9999);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
