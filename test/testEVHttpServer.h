#ifndef _TEST_EVHTTP_SERVER_H_
#define _TEST_EVHTTP_SERVER_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"

/**
 * @brief test init
 */
TEST(testEVHttpServer, testInit)
{
    EVHttpServer server1;
    EXPECT_EQ(server1.init(9999, "0.0.0.0"), true);

    EVHttpServer server2;
    EXPECT_EQ(server2.init(8888), true);
}

/**
 * @brief test init multiple ports
 */
TEST(testEVHttpServer, testInitMultiplePorts)
{
    EVHttpServer server;
    EXPECT_EQ(server.init({7777, 8888, 9999}, "0.0.0.0"), true);
}

/**
 * @brief test start
 */
TEST(testEVHttpServer, testStart)
{
    EVHttpServer server1;
    EXPECT_EQ(server1.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server1.start(), true);

    EVHttpServer server2;
    EXPECT_EQ(server2.init(8888), true);
    EXPECT_EQ(server2.start(5), true);

    EVHttpServer server3;
    EXPECT_EQ(server3.init(7777), true);
    EXPECT_EQ(server3.start(0), true);
}

/**
 * @brief test stop
 */
TEST(testEVHttpServer, testStop)
{
    EVHttpServer server;
    EXPECT_EQ(server.init(9999, "0.0.0.0"), true);
    EXPECT_EQ(server.start(5), true);
    EXPECT_EQ(server.stop(), true);
}

/**
 * @brief Test multiple calls to the same function
 */
TEST(testEVHttpServer, testMultipleCall)
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
    ASSERT_EQ(server.start(5), true);
    EXPECT_EQ(server.start(5), false);

    /* Can not add handler for same query multiple times */
    class Handle
    {
    public:
        static void handleFunc(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
        {
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
 * @brief Test handler related functions
 */
TEST(testEVHttpServer, testHandlerFunctions)
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
