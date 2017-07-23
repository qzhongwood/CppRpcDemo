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

int main(int argc, char* argv[])
{
    string host;
    int port    = 16666;
    int numTask = 1000;

    int i = 1;
    while (i < argc)
    {
        string opt = argv[i];
        if (opt.compare("-h") == 0)
        {
            host = argv[++i];
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

    try
    {
        if (host.length() > 0)
        {
            runAsClient(host, port, numTask);
        }
        else
        {
            runAsServer(port);
        }
    }
    catch (Exception* e)
    {
        printf("Exception: %s\n", e->getMessage().c_str());
        delete e;
    }    
    catch (...)
    {
        printf("Exception happened!\n");
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

void runAsClient(const string& host, int port, int numTask) 
{
    networkInit();

    RemotingClientPtr client = new RemotingClientImpl();
#if 1
    client->start();
#else
    client->start(host, port);
#endif

    for (int i = 0; i < numTask; ++i)
    {
        RemotingCommandPtr cmd = produceRequest();
        char tmp[2];
        sprintf(tmp, "%d", (i % 3) + 1);
        cmd->setOpCode(tmp);
        cmd->setOpCode("2");
        cmd->setIndex(i + 1);
#if 1
        //RemotingCommandPtr response = client->asyncInvoke(host.c_str(), port, cmd);
        RemotingCommandPtr response = client->invoke(host.c_str(), port, cmd, 50000);
        if (response != NULL)
        {
            printf("Response: <%s>\n", response->toString().c_str());
        }
        else
        {
            //printf("Response: Failed!!!\n");
        }
#else
        client->asyncInvoke(cmd);
#endif
    }

    list<RemotingCommandPtr> cmdList;

    while (true)
    {
        client->fetchResponse(cmdList);
        if (cmdList.size() == numTask)
        {
            break;
        }
        Sleep(1);
    }

    client->stop();
}

void runAsServer(int port) 
{
    networkInit();
    RemotingServerPtr server = new RemotingServerImpl();
    server->start(port);
}

/*
HANDLE gDoneEvent;

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    if (lpParam == NULL)
    {
        printf("TimerRoutine lpParam is NULL\n");
    }
    else
    {
        // lpParam points to the argument; in this case it is an int

        printf("Timer routine called. Parameter is %d.\n", 
            *(int*)lpParam);
        if(TimerOrWaitFired)
        {
            printf("The wait timed out.\n");
        }
        else
        {
            printf("The wait event was signaled.\n");
        }
    }

    SetEvent(gDoneEvent);
}

int test()
{
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = NULL;
    int arg = 123;

    // Use an event object to track the TimerRoutine execution
    gDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == gDoneEvent)
    {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return 1;
    }

    // Create the timer queue.
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return 2;
    }

    // Set a timer to call the timer routine in 10 seconds.
    if (!CreateTimerQueueTimer( &hTimer, hTimerQueue, 
        (WAITORTIMERCALLBACK)TimerRoutine, &arg , 10000, 0, 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return 3;
    }

    // TODO: Do other useful work here 

    printf("Call timer routine in 10 seconds...\n");

    // Wait for the timer-queue thread to complete using an event 
    // object. The thread will signal the event at that time.

    if (WaitForSingleObject(gDoneEvent, INFINITE) != WAIT_OBJECT_0)
        printf("WaitForSingleObject failed (%d)\n", GetLastError());

    CloseHandle(gDoneEvent);

    // Delete all timers in the timer queue.
    if (!DeleteTimerQueue(hTimerQueue))
        printf("DeleteTimerQueue failed (%d)\n", GetLastError());

    return 0;
}*/