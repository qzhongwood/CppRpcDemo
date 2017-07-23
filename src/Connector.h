#pragma once

#include <map>
#include <string>
#include "Referable.h"
#include "Mutex.h"

class RemotingCommandHandler;
typedef Ref<RemotingCommandHandler> RemotingCommandHandlerPtr;

using namespace std;

class Channel;
typedef Ref<Channel> ChannelPtr;

class Connector
{
public:
    Connector();
    ~Connector();
    static ChannelPtr connect(string address, int port);

protected:
    static ChannelPtr establishChannel(string address, int port);
};
