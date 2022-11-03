#ifndef _TEST_HTTP_REGEX_MATCH_H_
#define _TEST_HTTP_REGEX_MATCH_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"
#include <string>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace TestRegexMatch
{

volatile bool flag = false;
std::mutex mtx;
std::unique_lock<std::mutex> locker(mtx);
std::condition_variable conditionVal;

/**
 * @brief test class HttpReq
 */
TEST(testRegexMatch, testRegexMatch)
{
    class Handle
    {
    public:
        static void postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            EXPECT_EQ(req.method(), EVHttpServer::REQ_POST);
            EXPECT_EQ(req.methodStr(), "POST");

            EXPECT_EQ(req.path(), "/api/postHandle777");

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
            req.queries(queryList);
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

            flag = true;
            conditionVal.notify_one();
        }
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            EXPECT_EQ(req.method(), EVHttpServer::REQ_PUT);
            EXPECT_EQ(req.methodStr(), "PUT");

            EXPECT_EQ(req.path(), "/api/putHandle88_66");

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
            req.queries(queryList);
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

            flag = true;
            conditionVal.notify_one();
        }
        static void getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            EXPECT_EQ(req.method(), EVHttpServer::REQ_GET);
            EXPECT_EQ(req.methodStr(), "GET");

            EXPECT_EQ(req.path(), "/api/getHandle7x");

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
            req.queries(queryList);
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
            flag = true;
            conditionVal.notify_one();
        }
        static void deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            EXPECT_EQ(req.method(), EVHttpServer::REQ_DELETE);
            EXPECT_EQ(req.methodStr(), "DELETE");

            EXPECT_EQ(req.path(), "/api/deleteHandleFunction");

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
            req.queries(queryList);
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

            flag = true;
            conditionVal.notify_one();
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_POST, "/api/postHandle[1-9]+"}, Handle::postHandle), true);
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_PUT, "/api/putHandle[1-9]+_[1-9]+"}, Handle::putHandle), true);
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_GET, "/api/getHandle[1-9]x$"}, Handle::getHandle), true);
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_DELETE, "/api/deleteHandle[A-z]+"}, Handle::deleteHandle), true);
    EXPECT_EQ(server.start(5), true);

    flag = false;
    std::string cmdPost = R"(curl -i "http://0.0.0.0:9999/api/postHandle777?system=ubuntu&test=passed"  \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X POST)";
    system(cmdPost.c_str());
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle88_66?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X PUT)";
    system(cmdPut.c_str());
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);
    
    flag = false;
    std::string cmdGet = R"(curl -i "http://0.0.0.0:9999/api/getHandle7x?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -X GET)";
    system(cmdGet.c_str());
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/deleteHandleFunction?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X DELETE)";
    system(cmdDelete.c_str());
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);
}

/**
 * @brief Test error regex
 */
TEST(testRegexMatch, testErrorRegex)
{
    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
        }
    };

    EVHttpServer server;

    /* Test add null handler */
    EXPECT_EQ(server.addRegHandler({EVHttpServer::REQ_POST, "/api/fun]}{["}, Handle::handleFunc, nullptr), false);
}

}
#endif
