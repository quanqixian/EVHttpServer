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
}

#endif
