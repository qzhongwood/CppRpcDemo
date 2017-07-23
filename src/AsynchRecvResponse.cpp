#include "StdAfx.h"
#include "AsynchRecvResponse.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"

AsynchRecvResponse::AsynchRecvResponse(ChannelPtr c, BufferPtr buf)
: AsynchResult(c, buf)
{
    //printf("new AsynchRecvResponse\n");

}

AsynchRecvResponse::~AsynchRecvResponse(void)
{
    //printf("~~~AsynchRecvResponse\n");
}

void AsynchRecvResponse::complete(size_t bytesTransferred,
                              int success,
                              const void *completionKey,
                              u_long error)
{
    //printf("AsynchRecvResponse::complete\n");

    // copy the data returned GetQueuedCompletionStatus
    this->bytesTransferred = bytesTransferred;
    this->success_           = success;
    this->completionKey    = completionKey;
    this->error_             = error;
    
    if (handler != 0)
    {
        handler->handleRecvResponse(this);
    }
}