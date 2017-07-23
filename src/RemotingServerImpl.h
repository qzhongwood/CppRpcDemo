#pragma once
#include "map"
#include "RemotingServer.h"

class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;

class RemotingCommand;
typedef Ref<RemotingCommand> RemotingCommandPtr;

using namespace std;

class RemotingServerImpl : public RemotingServer
{
public:
    RemotingServerImpl(void);
    ~RemotingServerImpl(void);

    // implement RemotingServer interface
    virtual void start(int port);
    virtual void registerProcessor(string opCode, RequestProcessorPtr requestProcessor,
        ExecutorServicePtr executorService);

    // implement RemotingCommandHandler interface
    virtual void onRemotingCommand(ChannelPtr channel, BufferPtr buffer);

protected:
    void onRemotingCommand(ChannelPtr channel, RemotingCommandPtr cmd);
    map<string, RequestProcessorPtr> processorMap;
};
