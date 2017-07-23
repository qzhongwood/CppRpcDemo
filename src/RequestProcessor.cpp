#include "StdAfx.h"
#include "RequestProcessor.h"
#include "ExecutorService.h"
#include "RemotingCommand.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"

RequestProcessor::RequestProcessor()
{
}

RequestProcessor::~RequestProcessor()
{
}

void RequestProcessor::setThreadPool(ExecutorServicePtr threadPool)
{
    if (threadPool != NULL)
    {
        pool = threadPool;
        pool->spawn();
    }
}

RemotingCommandPtr RequestProcessor::processRequest(ChannelPtr channel, RemotingCommandPtr request)
{
    RemotingCommandPtr response;
    if (pool == NULL)
    {
        response = process(channel, request);
    }
    else
    {
        AsyncTaskPtr task = procudeAsyncTask(channel, request);
        pool->submitTask(task);
    }

    return response;
}
