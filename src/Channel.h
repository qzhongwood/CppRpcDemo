#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "string"
#include "Referable.h"

using namespace std;

class AsynchResult;
typedef Ref<AsynchResult> AsynchResultPtr;

class Buffer;
typedef Ref<Buffer> BufferPtr;

class RemotingCommandHandler;
typedef Ref<RemotingCommandHandler> RemotingCommandHandlerPtr;

class Channel : public Referable
{
public:
    Channel(string address, int port, SOCKET sock);
    ~Channel(void);

    int asyncSend(BufferPtr sendbuf);
    int asyncRecv(BufferPtr recvbuf);

    SOCKET socket();

    void handleSendResponse(AsynchResultPtr result);
    void handleRecvResponse(AsynchResultPtr result);

    void setRemotingCommandHandler(RemotingCommandHandlerPtr handler);

    void shutdown();
private:
    string address;
    int    port;
    SOCKET sock;
    RemotingCommandHandlerPtr handler;
    bool closed;
    static AtomicCounter counter;
};

typedef Ref<Channel> ChannelPtr;
