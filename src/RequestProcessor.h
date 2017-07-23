#pragma once
#include "Referable.h"

class AsyncTask;
typedef Ref<AsyncTask> AsyncTaskPtr;

class RemotingCommand;
typedef Ref<RemotingCommand> RemotingCommandPtr;

class Channel;
typedef Ref<Channel> ChannelPtr;

class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;

class RequestProcessor : public Referable
{
public:
    RequestProcessor();
    virtual ~RequestProcessor();
    virtual RemotingCommandPtr processRequest(ChannelPtr channel, RemotingCommandPtr request);
    void setThreadPool(ExecutorServicePtr pool);

protected:
    virtual RemotingCommandPtr process(ChannelPtr channel, RemotingCommandPtr request) = 0;
    virtual AsyncTaskPtr procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request) = 0;

    ExecutorServicePtr pool;
};

typedef Ref<RequestProcessor> RequestProcessorPtr;