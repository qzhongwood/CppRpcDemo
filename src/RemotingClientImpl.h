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
    virtual RemotingCommandPtr asyncInvoke(string address, int port, RemotingCommandPtr command);

    // better performance interface
    virtual void start(string address, int port);
    virtual RemotingCommandPtr invoke(RemotingCommandPtr command, long timeOut);
    virtual RemotingCommandPtr asyncInvoke(RemotingCommandPtr command);

    virtual void stop();

    virtual void onRemotingCommand(ChannelPtr channel, BufferPtr buffer);
    void fetchResponse(list<RemotingCommandPtr>& cmdList);

protected:
    void execute(string address, int port, RemotingCommandPtr command);
    ChannelPtr getChannel(string address, int port);
    map<string, ChannelPtr> channelMap;
    Mutex channelMapMutex;

    Mutex eventMapMutex;
    map<int, EventPtr> eventMap;
    map<int, RemotingCommandPtr> responseMap;

    //ExecutorServicePtr pool;
    ChannelPtr currentChannel;
};
