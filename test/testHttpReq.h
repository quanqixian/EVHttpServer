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
        static bool postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_POST);
            EXPECT_EQ(req.methodStr(), "POST");

            EXPECT_EQ(req.url(), "/api/postHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            std::string value;
            EXPECT_EQ(req.findHeader("Content-Type", value), true);
            EXPECT_EQ(value, "application/json");

            EXPECT_EQ(req.findHeader("NoExist", value), false);

            *pFlag = true;
            return true;
        }
        static bool putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_PUT);
            EXPECT_EQ(req.methodStr(), "PUT");

            EXPECT_EQ(req.url(), "/api/putHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            *pFlag = true;
            return true;
        }
        static bool getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");

            EXPECT_EQ(req.url(), "/api/getHandle");

            *pFlag = true;
            return true;
        }
        static bool deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_DELETE);
            EXPECT_EQ(req.methodStr(), "DELETE");

            EXPECT_EQ(req.url(), "/api/deleteHandle");

            std::string body = R"({"name":"tom"})";
            EXPECT_EQ(req.body(), body);

            *pFlag = true;
            return true;
        }
    };

    volatile bool flag = false;
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_POST, "/api/postHandle"}, Handle::postHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_PUT, "/api/putHandle"}, Handle::putHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_GET, "/api/getHandle"}, Handle::getHandle, (void *)&flag), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_DELETE, "/api/deleteHandle"}, Handle::deleteHandle, (void *)&flag), true);
    EXPECT_EQ(server.start(5), true);

    flag = false;
    std::string cmdPost = R"(curl -i "http://0.0.0.0:9999/api/postHandle"  \
                -H "Content-Type: application/json" \
                -d "{\"name\":\"tom\"}" -X POST)";
    system(cmdPost.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle" -d "{\"name\":\"tom\"}" -X PUT)";
    system(cmdPut.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    
    flag = false;
    std::string cmdGet = R"(curl -i "http://0.0.0.0:9999/api/getHandle" -X GET)";
    system(cmdGet.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/deleteHandle" -d "{\"name\":\"tom\"}" -X DELETE)";
    system(cmdDelete.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
}

#endif
