#ifndef _TEST_HTTP_RES_H_
#define _TEST_HTTP_RES_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <cstring>
#include <condition_variable>

namespace TestHttpRes
{

volatile bool flag = false;
std::mutex mtx;
std::unique_lock<std::mutex> locker(mtx);
std::condition_variable conditionVal;


/**
 * @brief test class HttpRes
 */
TEST(testHttpRes, testHttpRes)
{
    class Handle
    {
    public:
        static void postHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
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

            EXPECT_EQ(res.setBody("hello"), true);
            /* not allow setBody more than once */
            EXPECT_EQ(res.setBody("world"), false);

            flag = true;
            conditionVal.notify_one();
        }
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
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

            res.addHeader({"favouriteFood", "Udon"});

            flag = true;
            conditionVal.notify_one();
        }
        static void getHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
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
            res.setCode(503);
            res.setReason("TheReasonIsServiceUnavailable");

            flag = true;
            conditionVal.notify_one();
        }
        static void deleteHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
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
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/postHandle"}, Handle::postHandle), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, Handle::putHandle), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_GET, "/api/getHandle"}, Handle::getHandle), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_DELETE, "/api/deleteHandle"}, Handle::deleteHandle), true);
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "http_code:503"), nullptr);
    EXPECT_NE(strstr(buf, "TheReasonIsServiceUnavailable"), nullptr);

    flag = false;
    std::string cmdDelete = R"(curl -i "http://0.0.0.0:9999/api/deleteHandle?system=ubuntu&test=passed" \
                -H "Content-Type: application/json" \
                -H "Server: Apache" \
                -d "{\"name\":\"tom\"}" -X DELETE)";
    system(cmdDelete.c_str());
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
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
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            std::list<EVHttpServer::HttpKeyVal> list;
            list.push_back({"favouriteFood", "Udon"});
            list.push_back({"Specialty", "Drawing"});
            res.addHeaders(list);

            flag = true;
            conditionVal.notify_one();
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, Handle::putHandle), true);
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "favouriteFood"), nullptr);
    EXPECT_NE(strstr(buf, "Udon"), nullptr);
    EXPECT_NE(strstr(buf, "Specialty"), nullptr);
    EXPECT_NE(strstr(buf, "Drawing"), nullptr);
}

/**
 * @brief test setHeaders
 */
TEST(testHttpRes, setHeaders)
{
    class Handle
    {
    public:
        static void putHandle(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            std::list<EVHttpServer::HttpKeyVal> list;
            list.push_back({"favouriteFood", "Udon"});
            list.push_back({"Specialty", "Drawing"});
            res.setHeaders(list);

            flag = true;
            conditionVal.notify_one();
        }
    };

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, Handle::putHandle), true);
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
    conditionVal.wait_for(locker, std::chrono::seconds(1), []{return flag;});
    EXPECT_EQ(flag, true);
    EXPECT_NE(strstr(buf, "favouriteFood"), nullptr);
    EXPECT_NE(strstr(buf, "Udon"), nullptr);
    EXPECT_NE(strstr(buf, "Specialty"), nullptr);
    EXPECT_NE(strstr(buf, "Drawing"), nullptr);
}

/**
 * @brief test setBody use vector 
 */
TEST(testHttpRes, setBody_vector)
{
    volatile bool m_flag = false;
    std::mutex m_mtx;
    std::unique_lock<std::mutex> m_locker(m_mtx);
    std::condition_variable m_conditionVal;

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_POST, "/api/postHandle"}, [&](const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)->void {

        std::string str = "hello";
        std::vector<char> body(str.begin(), str.end());

        EXPECT_EQ(res.setBody(body), true);
        /* not allow setBody more than once */
        EXPECT_EQ(res.setBody(body), false);

        m_flag = true;
        m_conditionVal.notify_one();
    }), true);

    EXPECT_EQ(server.start(5), true);

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
    conditionVal.wait_for(locker, std::chrono::seconds(1), [&]{return m_flag;});
    EXPECT_EQ(m_flag, true);
    EXPECT_EQ(std::string(buf), "hello");
}

}
#endif
