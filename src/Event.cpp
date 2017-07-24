#include "stdafx.h"
#include "Event.h"
#include <iostream>

//#define rpcprintf printf

Event::Event(bool manualRet)
{
    handle = CreateEvent(NULL,               // default security attributes
        manualRet ? TRUE : FALSE,             // manual-reset event
        0,              // initial state is nonsignaled
        TEXT("RPC_EVENT")   // object name
        ); 
}

Event::~Event(void)
{
    ::CloseHandle(handle);
}

void Event::await(int milliseconds) const
{
    rpcprintf("Event::await, %x\n", this); 
    DWORD res = WaitForSingleObject(handle, milliseconds);
    switch (res) 
    {
    case WAIT_TIMEOUT: 
        rpcprintf("Event: Thread %d WAIT_TIMEOUT, %x\n", GetCurrentThreadId(), this);
        break;         
    case WAIT_OBJECT_0: 
        // Event object was signaled
        rpcprintf("Event: Thread %d signaled, %x\n", GetCurrentThreadId(), this);
        break; 
        // An error occurred
    default: 
        rpcprintf("Wait error: %d, %x\n", GetLastError(), this); 
    }
}


void Event::signal() const
{
    rpcprintf("Event::signal: %x\n", this); 
    ::SetEvent(handle);
}

void Event::reset() const
{
    ::ResetEvent(handle);
}
