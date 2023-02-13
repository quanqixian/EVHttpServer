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
 * @brief      Read file to buffer
 * @param[in]  fileName : name of file
 * @return     a smart pointer to buffer
 */
std::unique_ptr<char[]> readFile(const std::string & fileName)
{
    std::ifstream ifStm (fileName, std::ifstream::binary);
    if(ifStm.is_open())
    {
        std::filebuf * pBuf = ifStm.rdbuf();
        std::size_t size = pBuf->pubseekoff(0, ifStm.end, ifStm.in);
        pBuf->pubseekpos(0, ifStm.in);

        std::unique_ptr<char[]> buffer(new char[size + 1]);
        buffer[size] = 0;
        pBuf->sgetn(buffer.get(),size);
        ifStm.close();
        return buffer;
    }
    else
    {
        std::cout << "fail to open file:" << fileName;
        return nullptr;
    }
}

/**
 * @brief      Login callback handler
 * @param[in]  req : http request
 * @param[out] res : http response
 * @param[in]  arg : User-defined parameters
 * @return     void
 */
void loginCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::unique_ptr<char[]> buffer = readFile("./html/Login.html");
    if(nullptr != buffer.get())
    {
        res.addHeader({"Content-Type", "text/html;charset:utf-8;"});
        res.setBody(buffer.get());
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
        std::unique_ptr<char[]> buffer = readFile("./html/Configure.html");
        if(nullptr != buffer.get())
        {
            res.setBody(buffer.get());
        }
        else
        {
            res.setCode(404);
        }
    }
    else
    {
        std::unique_ptr<char[]> buffer = readFile("./html/ReLogin.html");
        if(nullptr != buffer.get())
        {
            res.setBody(buffer.get());
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
        std::unique_ptr<char[]> buffer = readFile("./html/Success.html");
        if(nullptr != buffer.get())
        {
            res.setBody(buffer.get());
        }
        else
        {
            res.setCode(404);
        }
    }
    else
    {
        std::unique_ptr<char[]> buffer = readFile("./html/Fail.html");
        if(nullptr != buffer.get())
        {
            res.setBody(buffer.get());
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

    server.addHandler({EVHttpServer::REQ_GET, "/"}, loginCallback);
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
