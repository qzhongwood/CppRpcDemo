#include "StdAfx.h"

#include <WinSock2.h>
#include <Windows.h>

#include <vector>
#include <iostream>

#include "Acceptor.h"
#include "Channel.h"
#include "NetworkIoEngine.h"
#include "Buffer.h"
#include "RemotingServer.h"
#include "Exception.h"

class Channel;
typedef Ref<Channel> ChannelPtr;

Acceptor::Acceptor()
: stopped(false)
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::setRemotingCommandHandler(RemotingCommandHandlerPtr handler)
{
    this->handler = handler;
}
void Acceptor::stop()
{
    closesocket(srvSocket);
    stopped = true;
}

int Acceptor::start(int port)
{
    srvSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    SOCKADDR_IN srvAddr;
    srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(port);
    int bindResult = bind(srvSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
    if (SOCKET_ERROR == bindResult) 
    {
        info_printf("Bind failed. Error: %d\n", GetLastError());
        throw new Exception("Bind failed.");
    }

    int listenResult = listen(srvSocket, 10);
    if (SOCKET_ERROR == listenResult)
    {
        info_printf("Listen failed. Error: %d\n", GetLastError());
        throw new Exception("Listen failed.");
    }

    info_printf("Listening on port %d ...\n", port);

    while (!stopped) 
    {
        SOCKADDR_IN saRemote;

        int RemoteLen = sizeof(saRemote);
        SOCKET acceptSocket = accept(srvSocket, (SOCKADDR*)&saRemote, &RemoteLen);
        info_printf("Here comes socket: %d\n", acceptSocket);
        if (SOCKET_ERROR == acceptSocket) 
        { 
            info_printf("Accept Error: %d \n", GetLastError());
            throw new Exception("Accept failed.");
        }

        ChannelPtr channel = new Channel("localhost", port, acceptSocket);
        channel->setRemotingCommandHandler(handler);
       
        NetworkIoEngine::instance()->registerHandle(channel->socket(), 0);
        // try to receive the 1st command header for the channel
        BufferPtr buf = new Buffer(sizeof(size_t));
        channel->asyncRecv(buf);
    }

    return 0;
}
