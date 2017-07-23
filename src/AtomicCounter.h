#pragma once
#include <windows.h>
class AtomicCounter
{
public:
    AtomicCounter(long value = 0);
    ~AtomicCounter(void);

    inline int operator++(int) 
    { 
        return InterlockedIncrement(&counter_) - 1; 
    }

    inline int operator--(int) 
    { 
        return InterlockedDecrement(&counter_) + 1; 
    }

    inline int operator++(void) 
    {
        return InterlockedIncrement(&counter_); 
    }
    inline int operator--(void) 
    { 
        return InterlockedDecrement(&counter_); 
    }

    inline int operator+=(long value) 
    {
        return ::InterlockedExchangeAdd(&counter_, value);
    }

    inline void operator=(long value) 
    { 
        ::InterlockedExchange(&counter_, value);
    }

    inline operator long() const 
    { 
        return ::InterlockedExchangeAdd(&counter_, 0); 
    }

private:
    mutable long counter_;
};
