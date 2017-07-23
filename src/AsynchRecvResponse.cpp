#include "StdAfx.h"
#include "AsynchRecvResponse.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"

AsynchRecvResponse::AsynchRecvResponse(ChannelPtr c, BufferPtr buf)
: AsynchResult(c, buf)
{
}

AsynchRecvResponse::~AsynchRecvResponse(void)
{
}

void AsynchRecvResponse::onAsyncOperationCompleted(size_t bytesTransferred,
                              int success,
                              const void *completionKey,
                              u_long error)
{
    // copy the data returned GetQueuedCompletionStatus
    this->bytesTransferred = bytesTransferred;
    this->success           = success;
    this->completionKey     = completionKey;
    this->operationError    = error;
    
    if (handler != 0)
    {
        handler->handleRecvResponse(this);
    }
}