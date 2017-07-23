#include "StdAfx.h"
#include "AsynchSendResponse.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"

AsynchSendResponse::AsynchSendResponse(ChannelPtr c, BufferPtr buf)
: AsynchResult(c, buf)
{
    //rpcprintf("new AsynchSendResponse\n");
}

AsynchSendResponse::~AsynchSendResponse()
{
    //rpcprintf("~~~AsynchSendResponse\n");
}

void AsynchSendResponse::onAsyncOperationCompleted(size_t bytesTransferred,
                        int success,
                        const void *completionKey,
                        u_long error)
{
    // Copy the data which was returned by <GetQueuedCompletionStatus>.
    this->bytesTransferred = bytesTransferred;
    this->success = success;
    this->completionKey = completionKey;
    this->operationError = error;

    if (handler != 0)
    {
        handler->handleSendResponse(this);
    }
}