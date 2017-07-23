#include "StdAfx.h"
#include "Channel.h"
#include "AsynchRecvResponse.h"
#include "AsynchSendResponse.h"
#include "Buffer.h"
#include "RemotingCommandHandler.h"

AtomicCounter Channel::counter;

Channel::Channel(string address, int port, SOCKET sock)
: address(address)
, port(port)
, sock(sock)
, closed(false)
{
    long n = ++counter;
}

Channel::~Channel(void)
{
    int n = --counter;
    rpcprintf("Channel destruction: <%x>, counter <%d>\n", this, n);
    shutdown();
}

SOCKET Channel::socket()
{
    return sock;
}

void Channel::shutdown()
{
    handler = 0;
    closed = true;
    closesocket(sock);
}

void Channel::setRemotingCommandHandler(RemotingCommandHandlerPtr handler)
{
    this->handler = handler;
}

void Channel::handleRecvResponse(AsynchResultPtr res)
{
    if (res->getBytesTransferred() == 0)
    {
        rpcprintf("No data received. <%x>\n", this);
        return;
    }

    BufferPtr buf     = res->getBuffer();
    BufferPtr prevBuf = buf->prevBuffer();

    if (prevBuf == NULL) 
    {
        // res->getBuffer() is RemotingCommand header
        size_t length;
        buf->deserialize((char*)&length, sizeof(length));

        // issuing an async receive for RemotingCommand body
        BufferPtr body = new Buffer(length);
        buf->setNext(body);
        res->setBuffer(0);
        asyncRecv(body);
    }
    else
    {
        if (handler != NULL)
        {
            // notify the upper layer the buffer, since we got a whole command
            handler->onRemotingCommand(this, buf);
        }

        // issuing an async receive for next command header
        BufferPtr newBuf = new Buffer(sizeof(size_t));
        asyncRecv(newBuf);
    }    
}

int Channel::asyncRecv(BufferPtr buf)
{
    if (closed)
    {
        return -1;
    }

    DWORD Flags = 0; 
    AsynchResultPtr resp = new AsynchRecvResponse(this, buf);

    WSABUF databuff;
    databuff.buf = buf->getBuf();
    databuff.len = (u_long)buf->getLength();

    DWORD RecvBytes;
    resp->increaseReferenceCount();
    int res = WSARecv(sock, &(databuff), 1, &RecvBytes, &Flags, resp.get(), NULL);

    if (res != 0 && WSA_IO_PENDING != WSAGetLastError())
    {
        rpcprintf("WSARecv :%d, err: %d, refCounter: <%d>, this <%p>, this <%x>\n", 
            res, WSAGetLastError(), getReferenceCount(), this, this);
        shutdown();
    } 
    else
    {
        res = 0;
    }

    return res;
}

void Channel::handleSendResponse(AsynchResultPtr res)
{
    
}

int Channel::asyncSend(BufferPtr sendbuf)
{
    if (closed)
    {
        return -1;
    }

    DWORD Flags = 0;  
    AsynchResultPtr resp = new AsynchSendResponse(this, sendbuf);

    WSABUF databuff;
    databuff.buf = sendbuf->getBuf();
    databuff.len = (u_long)sendbuf->getLength();

    DWORD SendBytes = 0;
    resp->increaseReferenceCount();
    int res = ::WSASend(this->sock, &(databuff), 1, &SendBytes, Flags, resp.get(), NULL);

    if (res != 0 && WSA_IO_PENDING != WSAGetLastError())
    {
        rpcprintf("WSASend :%d, err: %d, refCounter: <%d>, this<%p>\n", 
            res, WSAGetLastError(), getReferenceCount(), this);
        shutdown();
    }
    else
    {
        res = 0;
    }
    return res;
}
