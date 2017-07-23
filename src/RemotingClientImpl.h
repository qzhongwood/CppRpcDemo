#pragma once
#include "map"
#include "RemotingClient.h"
#include "Mutex.h"
#include "Event.h"
using namespace std;
class Channel;
typedef Ref<Channel> ChannelPtr;

class Buffer;
typedef Ref<Buffer> BufferPtr;

class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;

class RemotingClientImpl : public RemotingClient
{
public:
    RemotingClientImpl(void);
    ~RemotingClientImpl(void);

    virtual void start();
    virtual RemotingCommandPtr invoke(string address, int port, RemotingCommandPtr command, long timeOut);

    // better performance interface
    virtual void start(string address, int port);
    virtual RemotingCommandPtr asyncInvoke(RemotingCommandPtr command);

    virtual void stop();

    virtual void onRemotingCommand(ChannelPtr channel, BufferPtr buffer);
    virtual void fetchResponse(list<RemotingCommandPtr>& cmdList);

protected:
    void submit(string address, int port, RemotingCommandPtr command);
    ChannelPtr getChannel(string address, int port);

    Mutex channelMapMutex;
    map<string, ChannelPtr> channelMap;

    Mutex eventMapMutex;
    map<int, EventPtr> eventMap;
    map<int, RemotingCommandPtr> responseMap;

    ChannelPtr currentChannel;
};
