#pragma once
#include "Mutex.h"

class Lock
{
public:
    Lock(const Mutex& mutex);
    ~Lock(void);

protected:
    const Mutex& mutex;
};
