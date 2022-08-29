#include "EVHttpServer.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <memory>
#include <fstream>
#include <cstring>

static volatile bool g_runFlag = true;

void sighandler(int signum)
{
    g_runFlag = false;
}

bool getQuery(const char * pStr, const char * key, std::string & value)
{
    bool ret = true;

    int len = strlen(key);
    char * p = strstr((char *)pStr, key);
    ret = ret && (nullptr != p);
    ret = ret && (nullptr != (p + len));
    ret = ret && ('=' == *(p + len));
    if(!ret)
    {
        return false;
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

std::unique_ptr<char[]> readFile(const std::string & fileName)
{
    std::ifstream ifStm (fileName, std::ifstream::binary);
    std::filebuf * pBuf = ifStm.rdbuf();
    std::size_t size = pBuf->pubseekoff(0, ifStm.end, ifStm.in);
    pBuf->pubseekpos(0, ifStm.in);

    std::unique_ptr<char[]> buffer(new char[size + 1]);
    buffer[size] = 0;
    pBuf->sgetn(buffer.get(),size);
    ifStm.close();
    return buffer;
}

void loginCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
    std::unique_ptr<char[]> buffer = readFile("./html/Login.html");

    res.addHeader({"Content-Type","text/html;charset:utf-8;"});
    res.setBody(buffer.get());
}

void checkLoginCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
	bool ret = true;
	std::string username;
	std::string password;

	ret = ret && getQuery(req.body().c_str(), "username", username);
	ret = ret && getQuery(req.body().c_str(), "password", password);
	ret = ret && (username == "admin");
	ret = ret && (password == "123456");
	if(!ret)
	{
		std::unique_ptr<char[]> buffer = readFile("./html/Login.html");
		res.setBody(buffer.get());
	}
	else
	{
		std::unique_ptr<char[]> buffer = readFile("./html/Configure.html");
		res.setBody(buffer.get());
	}
}

void saveCallback(const EVHttpServer::HttpReq & req, EVHttpServer::HttpRes & res, void * arg)
{
	bool ret = true;
	std::string parameterA;
	std::string parameterB;

	ret = ret && getQuery(req.body().c_str(), "parameterA", parameterA);
	ret = ret && getQuery(req.body().c_str(), "parameterB", parameterB);
	ret = ret && (parameterA == "123");
	ret = ret && (parameterB == "123");
	if(!ret)
	{
		std::unique_ptr<char[]> buffer = readFile("./html/Fail.html");
		res.setBody(buffer.get());
	}
	else
	{
		std::unique_ptr<char[]> buffer = readFile("./html/Success.html");
		res.setBody(buffer.get());
	}
}

int main(int argc, const char *argv[])
{
    EVHttpServer server;

    server.addHandler({EVHttpServer::REQ_GET, "/"}, loginCallback);
    server.addHandler({EVHttpServer::REQ_POST, "/checkLogin"}, checkLoginCallback);
    server.addHandler({EVHttpServer::REQ_POST, "/save"}, saveCallback);
    server.init(8080);
    server.start();

    signal(SIGINT, sighandler); 

    while(g_runFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
