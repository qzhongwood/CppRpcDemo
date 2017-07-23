#pragma once
#include "asynchresult.h"

class AsynchSendResponse : public AsynchResult
{
public:
    AsynchSendResponse(ChannelPtr c, BufferPtr buf);
    ~AsynchSendResponse();

    virtual void onAsyncOperationCompleted(size_t bytesTransferred,
        int success, const void *completionKey, u_long error);
};
