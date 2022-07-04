![EVHttpServer logo](./Logo01.jpg)

# Introduction

EVHttpServer is an http server implemented by encapsulating libevent using c++11, It provides:

- Simpler api

- Use thread pool to handle http requests

- Support regular matching url


# Examples 

Here is a simple example using EVHttpServer:

```c++
#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

bool func(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
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

    server.addHandler({EVHTTP_REQ_POST, "/api/fun"}, func);
    server.init(9999);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
```

See more examples [here](./example)

# Use in your project

The first way is to include the source code in the src directory into your project, and then give libevent's header file path, library path and rpath when compiling.

The second way is to use EVHTTPServer compiled as a library.
