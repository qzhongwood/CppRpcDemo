#include "StdAfx.h"
#include "Connector.h"
#include "Channel.h"
#include "Buffer.h"
#include "Lock.h"
#include "RemotingCommandHandler.h"
#include "NetworkIoEngine.h"
#include "Exception.h"

Connector::Connector()
{
}

Connector::~Connector(void)
{
}

ChannelPtr Connector::connect(string address, int port)
{
    char key[32];
    sprintf(key, "%s:%d", address.c_str(), port);

    ChannelPtr channel = establishChannel(address, port);
    if (channel != NULL)
    {
        int res = NetworkIoEngine::instance()->registerHandle(channel->socket(), 0);
        if (res == 0)
        {
            // try to receive the 1st command header for the channel
            BufferPtr buf = new Buffer(sizeof(size_t));
            channel->asyncRecv(buf);
        }
        else
        {
            info_printf("Register Handle Error: %d \n", GetLastError());
            channel->shutdown();
            channel = 0;
        }
    }
  
    return channel;
}

ChannelPtr Connector::establishChannel(string address, int port)
{
    ChannelPtr channel;

    SOCKET scok = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (scok == INVALID_SOCKET)
    {
        info_printf("invalid socket !");
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(address.c_str()); 
    if (::connect(scok, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        closesocket(scok);
        throw new Exception("Failed to establish connection!");
    }
    else
    {
        info_printf("Connection to %s:%d establisted!\n", address.c_str(), port);
        channel = new Channel(address, port, scok);
    }

    return channel;
}
