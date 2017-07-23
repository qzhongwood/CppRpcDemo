#pragma once
#include "Referable.h"
#include "Mutex.h"

class AsynchResult;
typedef Ref<AsynchResult> AsynchResultPtr;

class NetworkIoEngine;
typedef Ref<NetworkIoEngine> NetworkIoEnginePtr;

class NetworkIoEngine : public Referable
{
public:
    NetworkIoEngine(void);
    ~NetworkIoEngine(void);
    int init();
    int registerHandle(SOCKET handle, const void *completionKey);
    void handleEvents(unsigned long milli_seconds);

    static NetworkIoEnginePtr instance();

private:
    int createIocp();
    HANDLE completionPort;
    static NetworkIoEnginePtr engine;
    static Mutex mutex;
};
