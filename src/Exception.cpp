#include "Exception.h"

Exception::Exception(const string& msg)
: message(msg)
{
}

Exception::~Exception(void)
{
}


const string& Exception::getMessage()
{
    return message;
}
