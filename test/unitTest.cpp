#include "testEVHttpServer.h"
#include "testHttpReq.h"
#include "testHttpRes.h"
#include "testRegexMatch.h"
#include "testLambda.h"
#include "testWebSocket.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
