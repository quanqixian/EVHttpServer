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

namespace TestLambda
{

/**
 * @brief test lambda
 */
TEST(testLambda, test01)
{
    volatile bool m_flag = false;
    std::mutex m_mtx;
    std::unique_lock<std::mutex> m_locker(m_mtx);
    std::condition_variable m_conditionVal;

    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.addHandler({EVHttpServer::REQ_PUT, "/api/putHandle"}, [&](const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)->void {
        std::list<EVHttpServer::HttpKeyVal> list;
        list.push_back({"favouriteFood", "Udon"});
        list.push_back({"Specialty", "Drawing"});
        res.setHeaders(list);

        m_flag = true;
        m_conditionVal.notify_one();
    }), true);
    EXPECT_EQ(server.start(5), true);

    m_flag = false;
    char buf[512] = {0};
    std::string cmdPut = R"(curl -i "http://0.0.0.0:9999/api/putHandle?system=ubuntu&test=passed" \
                -d "{\"name\":\"tom\"}" -X PUT )";
    FILE *pFile = popen(cmdPut.c_str(), "r");
    ASSERT_NE(pFile, nullptr);
    fread(buf, 1, sizeof(buf) - 1, pFile);
    pclose(pFile);
    pFile = nullptr;
    conditionVal.wait_for(m_locker, std::chrono::seconds(1), [&]{return m_flag;});
    EXPECT_EQ(m_flag, true);
    EXPECT_NE(strstr(buf, "favouriteFood"), nullptr);
    EXPECT_NE(strstr(buf, "Udon"), nullptr);
    EXPECT_NE(strstr(buf, "Specialty"), nullptr);
    EXPECT_NE(strstr(buf, "Drawing"), nullptr);
}

}
#endif
