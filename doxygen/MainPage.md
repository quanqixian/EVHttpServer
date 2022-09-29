![EVHttpServer logo](./Logo01.jpg)

# 1.Introduction

EVHttpServer is just an http server implemented by encapsulating libevent using c++, It provides:

- Simpler api

- Use thread pool to handle http requests

- Support regular matching path

# 2.Tutorial

## 2.1 main classes to focus on:

Using EVHttpServer, there are three main classes to focus on:

- `EVHttpServer` : encapsulates HTTP service initialization, start, stop, and registration callbacks.
- `EVHttpServer::HttpReq` : encapsulates HTTP requests, and provides methods for obtaining headers, query, and body.
- `EVHttpServer::HttpRes` : encapsulates the http reply, and provides methods for setting headers and body.

## 2.2 demo

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

int main(int argc, const char *argv[])
{
    /* Create an http server */
    EVHttpServer server;
    
    /*  Initialize http server port */
    server.init(9999);

     /* start http server */
    server.start();

    /*
     *   Register the http request handler function, the combination of method and path represents a unique request.
     *   In the request processing function, various parameters of the request can be obtained through the req object,
     * and parameters such as the body and header of the reply can be set through the res object.
     */
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

    return 0;
}
```

