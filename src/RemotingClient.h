#pragma once
#include "list"
#include "string"
#include "Referable.h"
#include "RemotingCommandHandler.h"

class RemotingCommand;
typedef Ref<RemotingCommand> RemotingCommandPtr;

using namespace std;

class RemotingClient : public RemotingCommandHandler
{
public:
    virtual void start(string address, int port) = 0;
    virtual RemotingCommandPtr asyncInvoke(RemotingCommandPtr command) = 0;

    virtual void start() = 0;
    virtual RemotingCommandPtr invoke(string address, int port, RemotingCommandPtr command, long timeOut) = 0;

    virtual void stop() = 0;
    virtual void fetchResponse(list<RemotingCommandPtr>& cmdList) = 0;
};
