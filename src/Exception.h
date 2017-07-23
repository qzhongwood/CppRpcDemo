#pragma once
#include "string"
using namespace std;

class Exception
{
public:
    Exception(const string& msg);
    ~Exception(void);
    const string& getMessage();

protected:
    string message;
};
