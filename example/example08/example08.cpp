#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <memory>
#include <fstream>
#include <cstring>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#endif

static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

/**
 * @brief      Read file to vector
 * @param[in]  fileName : name of file
 * @param[out] body : file content
 * @return     vector for storing file content
 */
bool readFile(const std::string & fileName, std::vector<char> & body)
{
    std::ifstream ifStm (fileName, std::ifstream::binary);
    if(ifStm.is_open())
    {
        std::filebuf * pBuf = ifStm.rdbuf();
        std::size_t size = pBuf->pubseekoff(0, ifStm.end, ifStm.in);
        pBuf->pubseekpos(0, ifStm.in);

        body.resize(size);
        pBuf->sgetn(&body[0],size);
        ifStm.close();
        return true;
    }
    else
    {
        std::cout << "fail to open file:" << fileName;
        return false;
    }
}

/* Try to guess a good content-type for filePath */
static const char * getContentType(const std::string & filePath)
{
    struct TableEntry 
    {
        const char *extension;
        const char *content_type;
    };
    static const TableEntry content_type_table[] = {
        /* Image Type */
        { "jpg", "image/jpeg" },
        { "jpeg", "image/jpeg" },
        { "png", "image/png" },
        { "gif", "image/gif" },

        /* Text Type */
        { "html", "text/html" },
        { "htm", "text/htm" },
        { "css", "text/css" },
        { "txt", "text/plain" },
        { "c", "text/plain" },
        { "h", "text/plain" },

        /* Audio Type */
        { "wav", "audio/wav" },
        { "mp3", "audio/mp3" },

        /* Video Type */
        { "avi", "video/avi" },
        { "mp4", "video/mpeg4" },
        { "mpeg", "video/mpg" },
        { "wmv", "video/x-ms-wmv" },

        /* Application Type */
        { "pdf", "application/pdf" },
        { "zip", "application/zip" },
        { "ps", "application/postscript" },

        { NULL, NULL },
    };
    std::size_t found = filePath.find_last_of('.');
    if(std::string::npos == found)
    {
        goto NOT_FOUND; /* no exension */
    }

    {
        std::string fileExtension = filePath.substr(found+1);
        for (auto ent = &content_type_table[0]; ent->extension; ++ent)
        {
            if(fileExtension == ent->extension)
            {
                return ent->content_type;
            }
        }
    }

NOT_FOUND:
    return "application/misc";
}

/**
 * @brief      static page handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void staticFileCallBack(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::string path = req.path();
    std::string filePath= "." + path;

    std::vector<char> body;
    bool ret = readFile(filePath, body);
    if(ret)
    {
        std::string contentType = getContentType(filePath);
        res.addHeader({"Content-Type", contentType});
        res.setBody(body);
    }
    else
    {
        res.setCode(404);
    }
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
    server.addRegHandler({EVHttpServer::REQ_GET, "/html/*"}, staticFileCallBack);

    /* dynamic page */

    /* websocket request */
    EVHttpServer::WSCallback wsCB;
    wsCB.onOpen = [&](EVHttpServer::WSSession * pSession, void * arg)->void {
        std::string clientInfo = pSession->getClientIP() + ":" + std::to_string(pSession->getClientPort());
        std::string sendMsg = clientInfo + " connected";
        std::cout<< sendMsg <<std::endl;
        server.sendWSBroadcast(sendMsg.c_str(), sendMsg.size());
    };
    wsCB.onClose = [&](EVHttpServer::WSSession * pSession, void * arg)->void {
        std::string clientInfo = pSession->getClientIP() + ":" + std::to_string(pSession->getClientPort());
        std::string sendMsg = clientInfo + " disconnected";
        std::cout<< sendMsg <<std::endl;
        server.sendWSBroadcast(sendMsg.c_str(), sendMsg.size());
    };
    wsCB.onMsg = [&](EVHttpServer::WSSession * pSession, int type, const unsigned char *data, size_t len, void *arg)->void {
        std::string clientInfo = pSession->getClientIP() + ":" + std::to_string(pSession->getClientPort());
        std::string msg = std::string((char*)data, len);
        std::cout << clientInfo << " msg:"<<msg<<std::endl;
        std::string sendMsg = clientInfo + " msg: " + msg;
        server.sendWSBroadcast(sendMsg.c_str(), sendMsg.size());
    };
    server.addWSHandler("/ws", wsCB);

    server.init(8080);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
