#ifndef _TEST_HTTP_RES_H_
#define _TEST_HTTP_RES_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <cstring>

/**
 * @brief test class HttpRes
 */
TEST(testHttpRes, testHttpRes)
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

            EXPECT_EQ(res.setBody("hello"), true);
            /* not allow setBody more than once */
            EXPECT_EQ(res.setBody("world"), false);
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

            res.addHeader({"favouriteFood", "Udon"});
            *pFlag = true;
            return true;
        }
        static bool getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            EXPECT_EQ(req.method(), EVHTTP_REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");

            EXPECT_EQ(req.url(), "/api/getHandle");

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
            res.setCode(503);
            res.setReason("TheReasonIsServiceUnavailable");
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
    std::string cmdPost = R"(curl "http://0.0.0.0:9999/api/postHandle?system=ubuntu&test=passed"  \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X POST)";
    FILE * pFile = popen(cmdPost.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    char buf[256] = {0};
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    EXPECT_EQ(std::string(buf), "hello");

    flag = false;
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X PUT | grep "favouriteFood")";
    pFile = popen(cmdPut.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    memset(buf, 0, sizeof(buf));
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "favouriteFood"), nullptr);
    EXPECT_NE(strstr(buf, "Udon"), nullptr);
    
    flag = false;
    std::string cmdGet = R"(curl -i "http://0.0.0.0:9999/api/getHandle?system=ubuntu&test=passed" \
                -w "\nhttp_code:%{http_code}\n"\
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -X GET)";
    pFile = popen(cmdGet.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    memset(buf, 0, sizeof(buf));
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "http_code:503"), nullptr);
    EXPECT_NE(strstr(buf, "TheReasonIsServiceUnavailable"), nullptr);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/deleteHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X DELETE)";
    system(cmdDelete.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);

    /*
     * test no register request 
     */
    flag = false;
    std::string cmdNoExist = R"(curl -i "http://0.0.0.0:9999/api/noExist" \
                -w "\nhttp_code:%{http_code}\n"\
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -X GET)";
    pFile = popen(cmdNoExist.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    memset(buf, 0, sizeof(buf));
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, false);
    EXPECT_NE(strstr(buf, "http_code:404"), nullptr);
}

/**
 * @brief test addHeaders
 */
TEST(testHttpRes, addHeaders)
{
    class Handle
    {
    public:
        static bool putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            bool * pFlag = static_cast<bool *>(arg);
            std::list<EVHttpServer::HttpKeyVal> list;
            list.push_back({"favouriteFood", "Udon"});
            list.push_back({"Specialty", "Drawing"});
            res.addHeaders(list);
            *pFlag = true;
            return true; 
        }
    };

    volatile bool flag = false;
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_PUT, "/api/putHandle"}, Handle::putHandle, (void *)&flag), true);
    EXPECT_EQ(server.start(5), true);

    flag = false;
    char buf[512] = {0};
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle?system=ubuntu&test=passed" \
                -d "{\"name\":\"tom\"}" -X PUT )";
    FILE *pFile = popen(cmdPut.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "favouriteFood"), nullptr);
    EXPECT_NE(strstr(buf, "Udon"), nullptr);
    EXPECT_NE(strstr(buf, "Specialty"), nullptr);
    EXPECT_NE(strstr(buf, "Drawing"), nullptr);
}
#endif
