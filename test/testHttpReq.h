#ifndef _TEST_HTTP_REQ_H_
#define _TEST_HTTP_REQ_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"
#include <string>
#include <stdlib.h>
#include <thread>

/**
 * @brief test method
 */
TEST(testHttpReq, testMethod)
{
    class Handle
    {
    public:
        static bool postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_POST);
            EXPECT_EQ(req.methodStr(), "POST");
            *pFlag = true;
            return true;
        }
        static bool putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_PUT);
            EXPECT_EQ(req.methodStr(), "PUT");
            *pFlag = true;
            return true;
        }
        static bool getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");
            *pFlag = true;
            return true;
        }
        static bool deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_DELETE);
            EXPECT_EQ(req.methodStr(), "DELETE");
            *pFlag = true;
            return true;
        }
    };

    volatile bool flag = false;
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_POST, "/api/fun1"}, Handle::postHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_PUT, "/api/fun1"}, Handle::putHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_GET, "/api/fun1"}, Handle::getHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_DELETE, "/api/fun1"}, Handle::deleteHandle, (void *)&flag), true);
    EXPECT_EQ(server.start(5), true);

    flag = false;
    std::string cmdPost = R"(curl -i "http://0.0.0.0:9999/api/fun1" -d "{\"name\":\"tom\"}" -X POST)";
    system(cmdPost.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/fun1" -d "{\"name\":\"tom\"}" -X PUT)";
    system(cmdPut.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    
    flag = false;
    std::string cmdGet = R"(curl -i "http://0.0.0.0:9999/api/fun1" -X GET)";
    system(cmdGet.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/fun1" -X DELETE)";
    system(cmdGet.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
}

#endif
