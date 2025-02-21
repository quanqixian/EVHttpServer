#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <memory>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <time.h>
#include <cstdio>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

#define BOUNDARY "boundarydonotcross"
static volatile bool g_runFlag = true;

void sighandler(int signum) {
    g_runFlag = false;
}

/**
 * @brief      Read file to vector
 * @param[in]  fileName : name of file
 * @param[out] body : file content
 * @return     vector for storing file content
 */
bool readFile(const std::string & fileName, std::vector<char> & body) {
    std::ifstream ifStm(fileName, std::ifstream::binary);
    if (ifStm.is_open()) {
        std::filebuf * pBuf = ifStm.rdbuf();
        std::size_t size = pBuf->pubseekoff(0, ifStm.end, ifStm.in);
        pBuf->pubseekpos(0, ifStm.in);

        body.resize(size);
        pBuf->sgetn(&body[0], size);
        ifStm.close();
        return true;
    } else {
        std::cout << "fail to open file:" << fileName;
        return false;
    }
}

/**
 * @brief      Login callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void rootCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg) {
    std::vector<char> body;
    bool ret = readFile("./html08/index.html", body);
    if (ret) {
        res.addHeader({"Content-Type", "text/html;charset:utf-8;"});
        res.setBody(body);
    } else {
        res.setCode(404);
    }
}

/**
 * @brief      Get parameters callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void getMjpgStream(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg) {
    res.setCode(200);
    std::list<EVHttpServer::HttpKeyVal> list;
    list.push_back({"Access-Control-Allow-Origin", "*"});
    list.push_back({"Connection", "close"});
    list.push_back({"Server", "MJPG-Streamer"});
    list.push_back({"Cache-Control", "no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0"});
    list.push_back({"Pragma", "no-cache"});
    list.push_back({"Expires", "Mon, 3 Jan 2000 12:34:56 GMT"});
    list.push_back({"Pragma", "no-cache"});
    list.push_back({"Content-Type", std::string("multipart/x-mixed-replace;boundary=") + BOUNDARY});
    list.push_back({"Content-Length", " "});  // without this maybe chunked.
    res.setHeaders(list);

    std::cout << "Start send jpg stream..." << std::endl;

    res.start();
    std::string boundStart = std::string("--") + BOUNDARY + "\r\n";
    res.send(boundStart.c_str(), boundStart.size());
    int index = 0;
    while (g_runFlag && res.isConnect()) {
        std::vector<char> body;
        index = (index + 1) % 22;
        std::string fileNmae = std::string("./pic08/") + std::to_string(index) + ".jpg";
        if (!readFile(fileNmae, body)) {
            break;
        }

        struct timespec t = {0};
        clock_gettime(CLOCK_REALTIME, &t);

        char buffer[128] = {0};
        snprintf(buffer, sizeof(buffer), "Content-Type: image/jpeg\r\n" \
                "Content-Length: %u\r\n" \
                "X-Timestamp: %lu.%d\r\n" \
                "\r\n", body.size(), t.tv_sec, t.tv_nsec/1000000);
        res.send(buffer, strlen(buffer));

        res.send(&body[0], body.size());

        std::string bound = std::string("\r\n--") + BOUNDARY + "\r\n";
        res.send(bound.c_str(), bound.size());

        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    res.end();
    std::cout << "End send jpg stream." << std::endl;
}

int main(int argc, const char *argv[])
{
#ifdef _WIN32
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
}
#endif

    EVHttpServer server;

    /* static page or files */
    server.addHandler({EVHttpServer::REQ_GET, "/"}, rootCallback);

    /* dynamic page */
    server.addHandler({EVHttpServer::REQ_GET, "/mjpgStream"}, getMjpgStream);

    server.init(8080);
    server.start();

    signal(SIGINT, sighandler);

    while (g_runFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
