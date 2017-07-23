// cpprpc.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <winbase.h>
#include <stdio.h>
#include "RemotingClientImpl.h"
#include "RemotingCommand.h"
#include "NetworkIoEngine.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Channel.h"
#include "Buffer.h"
#include "RemotingServerImpl.h"
#include "ExecutorService.h"
#include "Exception.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")

class RemotingClient;
typedef Ref<RemotingClient> RemotingClientPtr;

class RemotingServer;
typedef Ref<RemotingServer> RemotingServerPtr;

using namespace std;

void runAsServer(int port);
void runAsClient(const string& host, int port, int numTask);
void runAsAsyncClient(const string& host, int port, int numTask);
void dumpResponse(RemotingCommandPtr response, const string& opcode);

void displayUsageAndExit()
{
    info_printf("Usage:\n");
    info_printf("    Run as server: rpcdemo.exe -p port\n");
    info_printf("    Run as sync client: rpcdemo.exe -p port -h ip -n numRequest\n");
    info_printf("    Run as async client: rpcdemo.exe -p port -h ip -n numRequest -a\n");
    exit(-1);
}

int main(int argc, char* argv[])
{
    string host;
    int port    = -1;
    int numTask = -1;
    bool asyncMode = false;

    if (argc != 3 && argc != 7 && argc != 8)
    {
        displayUsageAndExit();
    }

    int i = 1;
    while (i < argc)
    {
        string opt = argv[i];
        if (opt.compare("-h") == 0)
        {
            host = argv[++i];
        }

        if (opt.compare("-a") == 0)
        {
            asyncMode = true;
        }

        if (opt.compare("-p") == 0)
        {
            port = atoi(argv[++i]);
        }

        if (opt.compare("-n") == 0)
        {
            numTask = atoi(argv[++i]);
        }
        ++i;
    }

    if (port < 0)
    {
        displayUsageAndExit();
    }

    try
    {
        if (host.length() > 0)
        {
            if (numTask < 0)
            {
                displayUsageAndExit();
            }
            if (asyncMode)
            {
                runAsAsyncClient(host, port, numTask);
            }
            else
            {
                runAsClient(host, port, numTask);
            }
        }
        else
        {
            runAsServer(port);
        }
    }
    catch (Exception* e)
    {
        info_printf("Exception: %s\n", e->getMessage().c_str());
        delete e;
    }    
    catch (...)
    {
        info_printf("Exception happened!\n");
    }
  
    return 0;
}

void networkInit()
{
    int res = NetworkIoEngine::instance()->init();
    if (res != 0)
    {
        throw new Exception("Failed to initialize network IO engine.");
    }
}

RemotingCommandPtr produceRequest()
{
    RemotingCommandPtr cmd = new RemotingCommand();
    map<string, string> header;
    header["city"]     = "hangzhou";
    header["province"] = "zhejiang";
    header["country"]  = "china";
    cmd->setHeader(header);
    cmd->setOpCode("2");
    static char ali[] = "Alibaba";
    cmd->setPayload(ali, sizeof(ali));
    return cmd;
}

void runAsAsyncClient(const string& host, int port, int numTask) 
{
    networkInit();

    RemotingClientPtr client = new RemotingClientImpl();
    client->start(host, port);

    for (int i = 0; i < numTask; ++i)
    {
        RemotingCommandPtr cmd = produceRequest();
        char tmp[2];
        sprintf(tmp, "%d", (i % 3) + 1);
        cmd->setOpCode(tmp);
        cmd->setIndex(i + 1);

        info_printf("Submit request: opcode<%s>, index<%d>\n", 
            tmp, i + 1);

        client->asyncInvoke(cmd);
    }

    size_t size = 0;
    while (true)
    {
        list<RemotingCommandPtr> cmdList;
        client->fetchResponse(cmdList);
        size += cmdList.size();
        while (cmdList.size() > 0)
        {
            RemotingCommandPtr resp = *(cmdList.begin());
            cmdList.pop_front();
            dumpResponse(resp, "");
        }

        if (size == numTask)
        {
            break;
        }
        Sleep(1);
    }

    client->stop();
}


void runAsClient(const string& host, int port, int numTask) 
{
    networkInit();

    RemotingClientPtr client = new RemotingClientImpl();
    client->start();

    for (int i = 0; i < numTask; ++i)
    {
        RemotingCommandPtr cmd = produceRequest();
        char tmp[2];
        sprintf(tmp, "%d", (i % 3) + 1);
        cmd->setOpCode(tmp);
        cmd->setIndex(i + 1);

        info_printf("Submit request: index<%d>, opcode<%s>\n", 
            i + 1, tmp);

        RemotingCommandPtr response = client->invoke(host.c_str(), port, cmd, 60);
        dumpResponse(response, tmp);
    }

    client->stop();
}

void dumpResponse(RemotingCommandPtr response, const string& opcode)
{
    if (response != NULL)
    {
        info_printf("Response: <%s>\n", response->toString().c_str());
    }
    else
    {
        info_printf("Response: Failed!!! opcode<%s>\n", opcode.c_str());
    }
}

void runAsServer(int port) 
{
    networkInit();
    RemotingServerPtr server = new RemotingServerImpl();
    server->start(port);
}
