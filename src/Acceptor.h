#pragma once
#include "Referable.h"

class RemotingCommandHandler;
typedef Ref<RemotingCommandHandler> RemotingCommandHandlerPtr;

class Acceptor
{
public:
    Acceptor();
    ~Acceptor(void);
    int start(int port);
    void stop();
    void setRemotingCommandHandler(RemotingCommandHandlerPtr handler);

protected:
    RemotingCommandHandlerPtr handler;
    SOCKET srvSocket;
    bool stopped;
};
