#include "StdAfx.h"
#include "AsynchResult.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"

AtomicCounter AsynchResult::counter;
AsynchResult::AsynchResult(ChannelPtr c, BufferPtr b)
: handler(c)
, buffer(b)
{
    ++counter;
    printf("new AsynchResult\n");

    this->Internal = 0;
    this->InternalHigh = 0;
    this->Offset = 0;
    this->OffsetHigh = 0;
    this->hEvent = 0;
}

AsynchResult::~AsynchResult(void)
{
    int n = --counter;
    printf("~AsynchResult: <%x>, counter <%d>\n", this, n);
    if (buffer != NULL)
    {
        buffer->clear();
    }
}

BufferPtr AsynchResult::getBuffer()
{
    return buffer;
}

void AsynchResult::setBuffer(BufferPtr buf)
{
    this->buffer = buf;
}

size_t AsynchResult::getBytesTransferred (void) const
{
    return this->bytesTransferred;
}

int AsynchResult::success (void) const
{
    return this->success_;
}

u_long AsynchResult::error (void) const
{
    return this->error_;
}

HANDLE AsynchResult::event (void) const
{
    return this->hEvent;
}

u_long AsynchResult::offset (void) const
{
    return this->Offset;
}

u_long AsynchResult::offset_high (void) const
{
    return this->OffsetHigh;
}
