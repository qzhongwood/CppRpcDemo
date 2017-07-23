#include "StdAfx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <process.h>  
#include "ExecutorService.h"
#include "Lock.h"

ExecutorService::ExecutorService(const string& name, int n)
: stop(false)
, name(name)
, numThread(n)
, numTaskExecuted(0)
{
}

ExecutorService::~ExecutorService(void)
{
}

void  ExecutorService::submitTask(AsyncTaskPtr task)
{
    Lock lock(mutex);
    runnableTaskList.push_back(task);
    event.signal();
}

void ExecutorService::spawn()
{
    for (int i = 0; i < numThread; ++i) 
    {
        HANDLE ThreadHandle = CreateThread(NULL, 0, ExecutorService::TaskThread, this, 0, NULL);
        if (NULL == ThreadHandle)
        {
            rpcprintf("Create Thread Handle failed. Error: %d\n", GetLastError());
            break;
        }
        CloseHandle(ThreadHandle);
    }
}

bool ExecutorService::isStopped()
{
    return stop;
}

DWORD WINAPI ExecutorService::TaskThread(LPVOID IpParam)
{
    ExecutorServicePtr threadPool = (ExecutorService*)IpParam; 
    rpcprintf("<%s> working thread started!! Thread id<%d>\n", 
        threadPool->name.c_str(), GetCurrentThreadId());

    while (!threadPool->stop)
    {
        bool shouldWait = false;
        {
            Lock lock(threadPool->mutex);
            shouldWait = (threadPool->runnableTaskList.size() == 0) ? true : false;
            threadPool->event.reset();
        }

        if (shouldWait)
        {
            // wait for task enqueued
            threadPool->event.await();
        }

        AsyncTaskPtr task;
        {
            Lock lock(threadPool->mutex);
            // double check
            if (threadPool->runnableTaskList.size() > 0)
            {
                list<AsyncTaskPtr>::iterator it = threadPool->runnableTaskList.begin();
                task = *it;
                threadPool->runnableTaskList.erase(it);
                ++threadPool->numTaskExecuted;
            }
        }

        if (task != NULL)
        {
            task->SafeExecute();
        }
    }
    return 0;
}


            /*
            rpcprintf("<%s> working thread run task. numTaskExecuted<%d>. Thread id<%d>\n", 
                            threadPool->name.c_str(), 
                            threadPool->numTaskExecuted - 1,
                            GetCurrentThreadId());*/
            

/*
rpcprintf("<%s> working thread await... numTaskExecuted<%d>. Thread id<%d>\n", 
threadPool->name.c_str(), 
threadPool->numTaskExecuted,
GetCurrentThreadId());*/
/*
rpcprintf("<%s> working thread signaled!! Thread id<%d>\n", 
threadPool->name.c_str(), 
GetCurrentThreadId());*/
