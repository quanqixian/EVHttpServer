#ifndef _TEST_WEB_SOCKET_H_
#define _TEST_WEB_SOCKET_H_

#include "EVHttpServer.h"
#include "gtest/gtest.h"

namespace TestWebSocket
{

/**
 * @brief test class HttpReq
 */
TEST(testWebSocket, testHandlerFunctions)
{
    EVHttpServer server;
    EVHttpServer::WSCallback wsCB;

    EXPECT_EQ(server.addWSHandler("/api/fun1", wsCB), true);
    EXPECT_EQ(server.addWSHandler("/api/fun1", wsCB), false);

    EXPECT_EQ(server.rmWSHandler("/api/fun1"), true);
    EXPECT_EQ(server.rmWSHandler("/api/fun2"), false);
}

}

#endif
