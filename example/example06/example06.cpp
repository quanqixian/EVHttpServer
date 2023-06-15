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
static std::string g_parameterA = "test";
static std::string g_parameterB = "123";

void sighandler(int signum)
{
    g_runFlag = false;
}

/**
 * @brief      get query parameter from string
 * @param[in]  pStr : string pointer 
 * @param[in]  key : key to find
 * @param[out] value : use to save result
 * @retval     true : success
 * @retval     false : failed
 */
bool getQuery(const char * pStr, const char * key, std::string & value)
{
    bool ret = true;

    size_t len = strlen(key);
    std::string str = std::string(key) + "="; 
    char * p = strstr((char *)pStr, str.c_str());
    ret = ret && (nullptr != p);
    ret = ret && (nullptr != (p + len + 1));
    if(!ret)
    {
        return ret;
    }

    char *pBegin = p + len + 1;
    char *pEnd = pBegin;
    while(pEnd)
    {
        if((0 == *pEnd) || ('&' == *pEnd))
        {
            pEnd--; 
            break;
        }
        pEnd++;
    }
    if(pBegin >= pEnd)
    {
        value = "";
    }
    else
    {
        value = std::string(pBegin, pEnd - pBegin + 1);
    }

    return ret;
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

/**
 * @brief      Login callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void rootCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::vector<char> body;
    bool ret = readFile("./html/Login.html", body);
    if(ret)
    {
        res.addHeader({"Content-Type", "text/html;charset:utf-8;"});
        res.setBody(body);
    }
    else
    {
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
void getParametersCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    char buf[256] = {0};
    res.addHeader({"Content-Type", "application/json"});
    snprintf(buf, sizeof(buf), "{\"parameterA\":\"%s\",\"parameterB\":\"%s\"}", g_parameterA.c_str(), g_parameterB.c_str());
    res.setBody(buf);
}

/**
 * @brief      Check login callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void checkLoginCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    bool ret = true;
    std::string username;
    std::string password;

    ret = ret && getQuery(req.body().c_str(), "username", username);
    ret = ret && getQuery(req.body().c_str(), "password", password);
    ret = ret && (username == "admin");
    ret = ret && (password == "123456");
    if(ret)
    {
        std::vector<char> body;
        bool ret = readFile("./html/Configure.html", body);
        if(ret)
        {
            res.setBody(body);
        }
        else
        {
            res.setCode(404);
        }
    }
    else
    {
        std::vector<char> body;
        bool ret = readFile("./html/ReLogin.html", body);
        if(ret)
        {
            res.setBody(body);
        }
        else
        {
            res.setCode(404);
        }
    }
}

/**
 * @brief      Save parameters callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void saveCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    bool ret = true;
    std::string parameterA;
    std::string parameterB;

    std::string decodedBody;

    /* there need decode twice */
    req.decode(req.body(), decodedBody);
    req.decode(decodedBody, decodedBody);
    std::cout<<"before decode,body="<<req.body()<<std::endl;
    std::cout<<"after decode,body="<<decodedBody<<std::endl;

    ret = ret && getQuery(decodedBody.c_str(), "parameterA", parameterA);
    ret = ret && getQuery(decodedBody.c_str(), "parameterB", parameterB);
    if(ret)
    {
        g_parameterA = parameterA;
        g_parameterB = parameterB;
        std::vector<char> body;
        bool ret = readFile("./html/Success.html", body);
        if(ret)
        {
            res.setBody(body);
        }
        else
        {
            res.setCode(404);
        }
    }
    else
    {
        std::vector<char> body;
        bool ret = readFile("./html/Fail.html", body);
        if(ret)
        {
            res.setBody(body);
        }
        else
        {
            res.setCode(404);
        }
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
    server.addHandler({EVHttpServer::REQ_GET, "/"}, rootCallback);
    server.addRegHandler({EVHttpServer::REQ_GET, "/html/*"}, staticFileCallBack);

    /* dynamic page */
    server.addHandler({EVHttpServer::REQ_GET, "/getParameters"}, getParametersCallback);
    server.addHandler({EVHttpServer::REQ_POST, "/checkLogin"}, checkLoginCallback);
    server.addHandler({EVHttpServer::REQ_POST, "/save"}, saveCallback);

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
