#pragma once
#include <windows.h>
#include <string>
#include "Referable.h"
using namespace std;

class Event : public Referable
{
public:
    Event(const string& name, bool manualRet = true);
    Event(bool manualRet = true);
    ~Event(void);
    void await(int milliseconds = INFINITE) const;
    void signal() const;
    void reset() const;

protected:
    HANDLE handle; 
    string name;
};

typedef Ref<Event> EventPtr;