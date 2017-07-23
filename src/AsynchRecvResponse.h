#pragma once
#include "asynchresult.h"

class AsynchRecvResponse : public AsynchResult
{
public:
    AsynchRecvResponse(ChannelPtr c, BufferPtr buf);
    ~AsynchRecvResponse();

    virtual void complete(size_t bytesTransferred,
        int success, const void *completionKey, u_long error);

};
