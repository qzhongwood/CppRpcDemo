#include "stdafx.h"
#include "Event.h"
#include <iostream>

//#define rpcprintf printf

Event::Event(const string& n, bool manualRet)
:name(n)
{
    handle = CreateEvent(NULL,               // default security attributes
        manualRet ? TRUE : FALSE,             // manual-reset event
        0,              // initial state is nonsignaled
        LPCWSTR(name.c_str())   // object name
        ); 
}

Event::Event(bool manualRet)
{
    handle = CreateEvent(NULL,               // default security attributes
        manualRet ? TRUE : FALSE,             // manual-reset event
        0,              // initial state is nonsignaled
        0               // object name
        ); 
}

Event::~Event(void)
{
    ::CloseHandle(handle);
}

void Event::await(int milliseconds) const
{
    rpcprintf("Event::await<%s>: %x\n", name.c_str(), this); 
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
    rpcprintf("Event::signal<%s>: %x\n", name.c_str(), this); 
    ::SetEvent(handle);
}

void Event::reset() const
{
    ::ResetEvent(handle);
}
