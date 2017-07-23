#pragma once
#include <windows.h>

class Mutex
{
public:
    Mutex(void);
    ~Mutex(void);

    virtual void lock(int milliseconds = INFINITE) const;
    virtual void unlock() const;

private:
    HANDLE handle;
};