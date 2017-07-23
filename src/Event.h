#pragma once
#include <windows.h>
#include "Referable.h"

class Event : public Referable
{
public:
    Event(bool manualRet = true);
    ~Event(void);
    void await(int milliseconds = INFINITE) const;
    void signal() const;
    void reset() const;

protected:
    HANDLE handle; 
};

typedef Ref<Event> EventPtr;