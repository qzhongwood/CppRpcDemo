#include "StdAfx.h"
#include "Lock.h"

Lock::Lock(const Mutex& m)
: mutex(m)
{
    mutex.lock();
}

Lock::~Lock(void)
{
    mutex.unlock();
}
