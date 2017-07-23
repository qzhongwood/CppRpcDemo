#include "stdafx.h"
#include "Event.h"
#include <iostream>

Event::Event(bool manualRet)
{
    handle = CreateEventA(NULL,               // default security attributes
        manualRet ? TRUE : FALSE,
        //FALSE,               // manual-reset event
        0,              // initial state is nonsignaled
        ("RPC_EVENT")   // object name
        ); 
}

Event::~Event(void)
{
    ::CloseHandle(handle);
}

void Event::await(int milliseconds) const
{
    DWORD res = WaitForSingleObject(handle, milliseconds);
    switch (res) 
    {
    case WAIT_TIMEOUT: 
        rpcprintf("Event: Thread %d WAIT_TIMEOUT\n", GetCurrentThreadId());
        break;         
    case WAIT_OBJECT_0: 
        // Event object was signaled
        rpcprintf("Event: Thread %d signaled\n", GetCurrentThreadId());
        break; 
        // An error occurred
    default: 
        rpcprintf("Wait error (%d)\n", GetLastError()); 
    }
}


void Event::signal() const
{
    SetEvent(handle);
}

void Event::reset() const
{
    ResetEvent(handle);
}
