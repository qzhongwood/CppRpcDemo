#include "StdAfx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <WinBase.h>
#include <vector>
#include <iostream>
#include "NetworkIoEngine.h"
#include "Channel.h"
#include "AsynchResult.h"
#include "AsynchSendResponse.h"
#include "AsynchRecvResponse.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"
#include "Lock.h"
#include "Event.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")

NetworkIoEnginePtr NetworkIoEngine::engine = 0;
Mutex NetworkIoEngine::mutex;


DWORD WINAPI WorkingThread(LPVOID IpParam)
{
    rpcprintf("Network I/O working thread started. Thread id <%d>\n", GetCurrentThreadId());

    while (true)
    {
        NetworkIoEngine::instance()->handleEvents(INFINITE);
    }
    return 0;
}


NetworkIoEngine::NetworkIoEngine(void)
: completionPort(NULL)
{
}

NetworkIoEngine::~NetworkIoEngine(void)
{
}

int NetworkIoEngine::init()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;  
    DWORD err = WSAStartup(wVersionRequested, &wsaData);

    if (0 != err)
    { 
        rpcprintf("Request Windows Socket Library Error!\n");
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) 
    {
        WSACleanup();
        rpcprintf("Request Windows Socket Version 2.2 Error!\n");
        return -1;
    }

    return createIocp();
}

int NetworkIoEngine::createIocp()
{
    completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == completionPort) 
    {   
        rpcprintf("CreateIoCompletionPort failed. Error: %d\n", GetLastError());
        return -1;
    }

    SYSTEM_INFO osInfo;
    GetSystemInfo(&osInfo);
    int threadNum = (osInfo.dwNumberOfProcessors * 2);

    for (int i = 0; i < threadNum; ++i) 
    {
        HANDLE ThreadHandle = CreateThread(NULL, 0, WorkingThread, completionPort, 0, NULL);
        if (NULL == ThreadHandle)
        {
            rpcprintf("Create Thread Handle failed. Error: %d\n", GetLastError());
            return -1;
        }
        CloseHandle(ThreadHandle);
    }

    return 0;
}

NetworkIoEnginePtr NetworkIoEngine::instance()
{
    if (engine == NULL) 
    {
        Lock lock(mutex);
        if (engine == NULL) 
        {
            engine = new NetworkIoEngine();
        }
    }
    return engine;
}


int NetworkIoEngine::registerHandle(SOCKET handle, const void *completionKey)
{
    ULONG_PTR key (reinterpret_cast<ULONG_PTR> (completionKey));
    HANDLE cp = ::CreateIoCompletionPort((HANDLE)handle, this->completionPort, key, 0);

    if (cp == 0)
    {
        rpcprintf("Failed to register socket <%d>. Error: %d\n", 
            handle, GetLastError());
        return -1;
    }
    return 0;
}


void NetworkIoEngine::handleEvents(unsigned long milliSeconds)
{
    OVERLAPPED *overlapped  = 0;
    u_long bytesTransferred = 0;
    ULONG_PTR completionKey = 0;

    // get the next asynchronous operation that completes
    BOOL result = ::GetQueuedCompletionStatus(
        completionPort,
        &bytesTransferred,
        &completionKey,
        &overlapped,
        milliSeconds);

    if (result == TRUE && overlapped != 0)
    {
        AsynchResultPtr response = (AsynchResult*)overlapped;
        u_long resultErr = response->error();
        if (resultErr == 0)
        {   
            resultErr = errno;
        }

        response->onAsyncOperationCompleted(bytesTransferred, resultErr ? 0 : 1, 
            (void*)completionKey, resultErr);
        response->decreaseReferenceCount();
    }
}
