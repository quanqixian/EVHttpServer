#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

void func(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::cout << req.methodStr() << " " << req.path() << std::endl;
    std::cout << req.body() << std::endl;

    res.setBody(R"({"status":"OK"})");
    res.setCode(200);
}

int main(int argc, const char *argv[])
{
    EVHttpServer server;

    /* Add a handler for "regex path" */
    server.addRegHandler({EVHttpServer::REQ_POST, "/api/fun[1-9]+"}, func);
    server.init(9999);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
