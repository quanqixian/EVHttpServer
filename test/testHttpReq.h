#ifndef _TEST_HTTP_REQ_H_
#define _TEST_HTTP_REQ_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"
#include <string>
#include <stdlib.h>
#include <thread>

/**
 * @brief test class HttpReq
 */
TEST(testHttpReq, testHttpReq)
{
    class Handle
    {
    public:
        static void postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_POST);
            EXPECT_EQ(req.methodStr(), "POST");

            EXPECT_EQ(req.path(), "/api/postHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            std::string value;
            EXPECT_EQ(req.findHeader("Content-Type", value), true);
            EXPECT_EQ(value, "application/json");

            EXPECT_EQ(req.findHeader("Server", value), true);
            EXPECT_EQ(value, "Apache");

            EXPECT_EQ(req.findHeader("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> headerList;
            req.headers(headerList);
            EXPECT_GT(headerList.size(), 2);
            int matchCount = 0;
            for(auto iter = headerList.begin(); iter != headerList.end(); ++iter)
            {
                if(iter->key == "Content-Type")
                {
                    matchCount++;
                }
                if(iter->key == "Server")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            EXPECT_EQ(req.findQuery("system", value), true);
            EXPECT_EQ(value, "ubuntu");

            EXPECT_EQ(req.findQuery("test", value), true);
            EXPECT_EQ(value, "passed");

            EXPECT_EQ(req.findQuery("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> queryList;
            req.querys(queryList);
            EXPECT_EQ(queryList.size(), 2);

            matchCount = 0;
            for(auto iter = queryList.begin(); iter != queryList.end(); ++iter)
            {
                if(iter->key == "system")
                {
                    matchCount++;
                }
                if(iter->key == "test")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            *pFlag = true;
        }
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_PUT);
            EXPECT_EQ(req.methodStr(), "PUT");

            EXPECT_EQ(req.path(), "/api/putHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            std::string value;
            EXPECT_EQ(req.findHeader("Content-Type", value), true);
            EXPECT_EQ(value, "application/json");

            EXPECT_EQ(req.findHeader("Server", value), true);
            EXPECT_EQ(value, "Apache");

            EXPECT_EQ(req.findHeader("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> headerList;
            req.headers(headerList);
            EXPECT_GT(headerList.size(), 2);
            int matchCount = 0;
            for(auto iter = headerList.begin(); iter != headerList.end(); ++iter)
            {
                if(iter->key == "Content-Type")
                {
                    matchCount++;
                }
                if(iter->key == "Server")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            EXPECT_EQ(req.findQuery("system", value), true);
            EXPECT_EQ(value, "ubuntu");

            EXPECT_EQ(req.findQuery("test", value), true);
            EXPECT_EQ(value, "passed");

            EXPECT_EQ(req.findQuery("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> queryList;
            req.querys(queryList);
            EXPECT_EQ(queryList.size(), 2);

            matchCount = 0;
            for(auto iter = queryList.begin(); iter != queryList.end(); ++iter)
            {
                if(iter->key == "system")
                {
                    matchCount++;
                }
                if(iter->key == "test")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            *pFlag = true;
        }
        static void getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");

            EXPECT_EQ(req.path(), "/api/getHandle");

            std::string value;
            EXPECT_EQ(req.findHeader("Content-Type", value), true);
            EXPECT_EQ(value, "application/json");

            EXPECT_EQ(req.findHeader("Server", value), true);
            EXPECT_EQ(value, "Apache");

            EXPECT_EQ(req.findHeader("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> headerList;
            req.headers(headerList);
            EXPECT_GT(headerList.size(), 2);
            int matchCount = 0;
            for(auto iter = headerList.begin(); iter != headerList.end(); ++iter)
            {
                if(iter->key == "Content-Type")
                {
                    matchCount++;
                }
                if(iter->key == "Server")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            EXPECT_EQ(req.findQuery("system", value), true);
            EXPECT_EQ(value, "ubuntu");

            EXPECT_EQ(req.findQuery("test", value), true);
            EXPECT_EQ(value, "passed");

            EXPECT_EQ(req.findQuery("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> queryList;
            req.querys(queryList);
            EXPECT_EQ(queryList.size(), 2);

            matchCount = 0;
            for(auto iter = queryList.begin(); iter != queryList.end(); ++iter)
            {
                if(iter->key == "system")
                {
                    matchCount++;
                }
                if(iter->key == "test")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);
            *pFlag = true;
        }
        static void deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_DELETE);
            EXPECT_EQ(req.methodStr(), "DELETE");

            EXPECT_EQ(req.path(), "/api/deleteHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            std::string value;
            EXPECT_EQ(req.findHeader("Content-Type", value), true);
            EXPECT_EQ(value, "application/json");

            EXPECT_EQ(req.findHeader("Server", value), true);
            EXPECT_EQ(value, "Apache");

            EXPECT_EQ(req.findHeader("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> headerList;
            req.headers(headerList);
            EXPECT_GT(headerList.size(), 2);
            int matchCount = 0;
            for(auto iter = headerList.begin(); iter != headerList.end(); ++iter)
            {
                if(iter->key == "Content-Type")
                {
                    matchCount++;
                }
                if(iter->key == "Server")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            EXPECT_EQ(req.findQuery("system", value), true);
            EXPECT_EQ(value, "ubuntu");

            EXPECT_EQ(req.findQuery("test", value), true);
            EXPECT_EQ(value, "passed");

            EXPECT_EQ(req.findQuery("NoExist", value), false);

            std::list<EVHttpServer::HttpKeyVal> queryList;
            req.querys(queryList);
            EXPECT_EQ(queryList.size(), 2);

            matchCount = 0;
            for(auto iter = queryList.begin(); iter != queryList.end(); ++iter)
            {
                if(iter->key == "system")
                {
                    matchCount++;
                }
                if(iter->key == "test")
                {
                    matchCount++;
                }
            }
            EXPECT_EQ(matchCount, 2);

            *pFlag = true;
        }
    };

    volatile bool flag = false;
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/postHandle"}, Handle::postHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, Handle::putHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_GET, "/api/getHandle"}, Handle::getHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_DELETE, "/api/deleteHandle"}, Handle::deleteHandle, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    flag = false;
    std::string cmdPost = R"(curl -i "http://0.0.0.0:9999/api/postHandle?system=ubuntu&test=passed"  \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X POST)";
    system(cmdPost.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X PUT)";
    system(cmdPut.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    
    flag = false;
    std::string cmdGet = R"(curl -i "http://0.0.0.0:9999/api/getHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -X GET)";
    system(cmdGet.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/deleteHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X DELETE)";
    system(cmdDelete.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdNoExist = R"(curl -i "http://0.0.0.0:9999/api/noExist" -X POST)";
    system(cmdNoExist.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, false);
}

/**
 * @brief Test big body
 */
TEST(testHttpReq, testBigBody)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_GT(req.body().size(), 1024);
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    flag = false;
    std::string cmdPost = R"(curl "http://0.0.0.0:9999/api/fun" -d @./testHttpReq.h  -X POST)";
    system(cmdPost.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
}


/**
 * @brief test methodStr
 */
TEST(testHttpReq, methodStr)
{
    class Handle
    {
    public:
        static void postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_POST);
            EXPECT_EQ(req.methodStr(), "POST");
            *pFlag = true;
        }
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_PUT);
            EXPECT_EQ(req.methodStr(), "PUT");
            *pFlag = true;
        }
        static void getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");
            *pFlag = true;
        }
        static void deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_DELETE);
            EXPECT_EQ(req.methodStr(), "DELETE");
            *pFlag = true;
        }
        static void headHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_HEAD);
            EXPECT_EQ(req.methodStr(), "HEAD");
            *pFlag = true;
        }
        static void traceHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_TRACE);
            EXPECT_EQ(req.methodStr(), "TRACE");
            *pFlag = true;
        }
        static void connectHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_CONNECT);
            EXPECT_EQ(req.methodStr(), "CONNECT");
            *pFlag = true;
        }
        static void patchHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_PATCH);
            EXPECT_EQ(req.methodStr(), "PATCH");
            *pFlag = true;
        }
        static void optionsHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHttpServer::REQ_OPTIONS);
            EXPECT_EQ(req.methodStr(), "OPTIONS");
            *pFlag = true;
        }
    };

    volatile bool flag = false;
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);

    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/postHandle"}, Handle::postHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, Handle::putHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_GET, "/api/getHandle"}, Handle::getHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_DELETE, "/api/deleteHandle"}, Handle::deleteHandle, (void *)&flag), true);

    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_OPTIONS, "/api/optionsHandle"}, Handle::optionsHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_TRACE, "/api/traceHandle"}, Handle::traceHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_CONNECT, "/api/connectHandle"}, Handle::connectHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_HEAD, "/api/headHandle"}, Handle::headHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PATCH, "/api/patchHandle"}, Handle::patchHandle, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/postHandle" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/putHandle" -X PUT )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/getHandle" -X GET )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/deleteHandle" -X DELETE )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/traceHandle" -X TRACE )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/patchHandle" -X PATCH )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/headHandle" -X HEAD --head )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/optionsHandle" -X OPTIONS )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

/**
 * @brief Test without thread pool
 */
TEST(testHttpReq, testNoThreadPool)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_POST, "/api/fun[1-9]+"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(0), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/fun" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/fun100" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

/**
 * @brief Test multiple ports
 */
TEST(testHttpReq, testMultilePorts)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init({7777, 8888, 9999}, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(0), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:7777/api/fun" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:8888/api/fun" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:9999/api/fun" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

/**
 * @brief Test get request host
 */
TEST(testHttpReq, testGetRequestHost)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.host(), "0.0.0.0");
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(7777), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:7777/api/fun" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

/**
 * @brief Test get request uri
 */
TEST(testHttpReq, testGetRequestUri)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.uri(), "/api/fun?name=Tom");
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(7777), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:7777/api/fun?name=Tom" -X POST )";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

/**
 * @brief test bodyRaw
 */
TEST(testHttpReq, testbodyRaw)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            std::string body = R"({"name":"tom"})";
            std::unique_ptr<char[]> ptr = req.bodyRaw();
            EXPECT_EQ(body, ptr.get());
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(7777), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:7777/api/fun?name=Tom" -d "{\"name\":\"tom\"}" -X POST)";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }

}

/**
 * @brief test decode uri
 */
TEST(testHttpReq, testDecodeUri)
{
    volatile bool flag = false;

    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            std::string urlExpect = R"(/api/fun?name=你好)";
            std::string url = req.uri();
            std::string urlDecode = req.uri(true);
            EXPECT_NE(urlExpect, url);
            EXPECT_EQ(urlExpect, urlDecode);
            *pFlag = true;
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(7777), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/fun"}, Handle::handleFunc, (void *)&flag), true);
    ASSERT_EQ(server.start(5), true);

    {
        flag = false;
        std::string cmd = R"(curl -i "http://0.0.0.0:7777/api/fun?name=%E4%BD%A0%E5%A5%BD" -d "{\"name\":\"tom\"}" -X POST)";
        system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(flag, true);
    }
}

#endif
