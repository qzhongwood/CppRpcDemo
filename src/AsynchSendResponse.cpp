#include "StdAfx.h"
#include "AsynchSendResponse.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"

AsynchSendResponse::AsynchSendResponse(ChannelPtr c, BufferPtr buf)
: AsynchResult(c, buf)
{
    //printf("new AsynchSendResponse\n");
}

AsynchSendResponse::~AsynchSendResponse()
{
    //printf("~~~AsynchSendResponse\n");
}

void AsynchSendResponse::complete(size_t bytesTransferred,
                        int success,
                        const void *completionKey,
                        u_long error)
{
    // Copy the data which was returned by <GetQueuedCompletionStatus>.
    this->bytesTransferred = bytesTransferred;
    this->success_ = success;
    this->completionKey = completionKey;
    this->error_ = error;

    if (handler != 0)
    {
        handler->handleSendResponse(this);
    }
}