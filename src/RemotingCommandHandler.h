#pragma once
#include "Referable.h"

class RemotingCommand;
typedef Ref<RemotingCommand> RemotingCommandPtr;

class Buffer;
typedef Ref<Buffer> BufferPtr;

class Channel;
typedef Ref<Channel> ChannelPtr;

class RemotingCommandHandler : public Referable
{
public:
    virtual void onRemotingCommand(ChannelPtr channel, BufferPtr buffer) = 0;
};
