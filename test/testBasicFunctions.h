#ifndef _TEST_BASIC_FUNCTIONS_H_
#define _TEST_BASIC_FUNCTIONS_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"

/**
 * @brief test auto release resources
 */
TEST(testBasicFunctions, testAutoReleaseResources)
{
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.start(5), true);
}

/**
 * @brief test stop
 */
TEST(testBasicFunctions, testStop)
{
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.start(5), true);
    EXPECT_EQ(server.stop(), true);
}

/**
 * @brief Test multiple calls to the same function
 */
TEST(testBasicFunctions, testMultipleCall)
{
    EVHttpServer server;

    /* Can not init multiple times */
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.init(9999, "0.0.0.0"), false);

    /* Can not start multiple times */
    EXPECT_EQ(server.start(5), true);
    EXPECT_EQ(server.start(5), false);

    /* Can stop multiple times */
    EXPECT_EQ(server.stop(), true);
    EXPECT_EQ(server.stop(), true);

    /* Can not start multiple times */
    EXPECT_EQ(server.start(5), true);
    EXPECT_EQ(server.start(5), false);

    /* Can not add handler for same query multiple times */
    class Handle
    {
    public:
        static bool handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
            return true;
        }
    };
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_POST, "/api/fun1"}, Handle::handleFunc, nullptr), true);
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_POST, "/api/fun1"}, Handle::handleFunc, nullptr), false);
    EXPECT_EQ(server.rmHandler({EVHTTP_REQ_POST, "/api/fun1"}), true);
    EXPECT_EQ(server.rmHandler({EVHTTP_REQ_POST, "/api/fun1"}), false);

    EXPECT_EQ(server.addRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}, Handle::handleFunc, nullptr), true);
    EXPECT_EQ(server.addRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}, Handle::handleFunc, nullptr), false);
    EXPECT_EQ(server.rmRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}), true);
    EXPECT_EQ(server.rmRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}), false);

}

/**
 * @brief Test error argument
 */
TEST(testBasicFunctions, testErrorArg)
{
    EVHttpServer server;

    /* Test add null handler */
    EXPECT_EQ(server.addHandler({EVHTTP_REQ_POST, "/api/fun1"}, nullptr, nullptr), false);
    EXPECT_EQ(server.addRegHandler({EVHTTP_REQ_POST, "/api/fun[1-9]+"}, nullptr, nullptr), false);

    /* Test removal of unadded requests */
    EXPECT_EQ(server.rmHandler({EVHTTP_REQ_POST, "/api/fun2"}), false);
    EXPECT_EQ(server.rmRegHandler({EVHTTP_REQ_POST, "/api/fun2[1-9]+"}), false);
}

#endif
