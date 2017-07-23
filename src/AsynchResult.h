#pragma once
#include <WinSock2.h>
#include "Referable.h"

class Channel;
typedef Ref<Channel> ChannelPtr;

class Buffer;
typedef Ref<Buffer> BufferPtr;

class AsynchResult : public Referable, public OVERLAPPED
{
public:
    AsynchResult(ChannelPtr channel, BufferPtr buf);
    ~AsynchResult(void);

    virtual void onAsyncOperationCompleted(size_t bytesTransferred,
        int success,
        const void *completionKey,
        u_long error) = 0;

    // Number of bytes transferred by the operation.
    size_t getBytesTransferred(void) const;

    // Did the operation succeed?
    int isSuccess (void) const;

    // Error value if the operation fail.
    u_long error (void) const;

    BufferPtr getBuffer();
    void setBuffer(BufferPtr buf);

protected:

    /// Bytes transferred by this operation.
    size_t bytesTransferred;

    /// Success indicator.
    int success;

    /// ACT associated with handle.
    const void *completionKey;

    /// Error if operation failed.
    u_long operationError;

    ChannelPtr handler;
    BufferPtr  buffer;
    static AtomicCounter counter;
};
