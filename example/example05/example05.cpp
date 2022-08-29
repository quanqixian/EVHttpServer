#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>

static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

int main(int argc, const char *argv[])
{
	EVHttpServer server;

	server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, [](const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)->void {
			std::cout << req.methodStr() << " " << req.path() << std::endl;
			std::cout << req.body() << std::endl;

			res.setBody(R"({"status":"OK"})");
			});
    server.init(9999);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
