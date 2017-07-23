#include "StdAfx.h"
#include "Mutex.h"

Mutex::Mutex(void)
{
    handle = ::CreateMutex(NULL, FALSE, NULL);
}

Mutex::~Mutex(void)
{
    ::CloseHandle(handle);
}

void Mutex::lock(int milliseconds) const
{
    ::WaitForSingleObject(handle, milliseconds);
}

void Mutex::unlock(void) const
{
    ::ReleaseMutex(handle);
}