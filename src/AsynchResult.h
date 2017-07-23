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

    virtual void complete(size_t bytesTransferred,
        int success,
        const void *completionKey,
        u_long error) = 0;

    /// Number of bytes transferred by the operation.
    size_t getBytesTransferred(void) const;

    /// Did the operation succeed?
    int success (void) const;

    /// Error value if the operation fail.
    u_long error (void) const;

    /// Event associated with the OVERLAPPED structure.
    HANDLE event (void) const;

    /// This really make sense only when doing file I/O.
    u_long offset (void) const;

    /// Offset_high associated with the OVERLAPPED structure.
    u_long offset_high (void) const;

    /// Returns 0.
    int signal_number (void) const;

    /// Simulate error value to use in the post_completion ()
    void set_error (u_long errcode);

    /// Simulate value to use in the post_completion ()
    void setBytesTransferred (size_t nbytes);

    BufferPtr getBuffer();
    void setBuffer(BufferPtr buf);

protected:

    /// Bytes transferred by this operation.
    size_t bytesTransferred;

    /// Success indicator.
    int success_;

    /// ACT associated with handle.
    const void *completionKey;

    /// Error if operation failed.
    u_long error_;

    ChannelPtr handler;
    BufferPtr  buffer;
    static AtomicCounter counter;
};
