#include "StdAfx.h"
#include "AntRequestProcessor.h"
#include "RemotingCommand.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"
#include "ExecutorService.h"

AntRequestProcessor::AntRequestProcessor(void)
{
}

AntRequestProcessor::~AntRequestProcessor(void)
{
}

AsyncTaskPtr AntRequestProcessor::procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request)
{
    return ThreadStartInfo(
        this, &AntRequestProcessor::process, 
        channel, request);
}

RemotingCommandPtr AntRequestProcessor::process(ChannelPtr channel, RemotingCommandPtr request)
{
    RemotingCommandPtr response = new RemotingCommand();
    response->setOpCode(request->getOpCode());
    response->setResultCode(0);

    response->setIndex(request->getIndex());

    rpcprintf("Ant processor working. Thread Id <%d>\n", GetCurrentThreadId());

    char body[] = "This is from AntRequestProcessor";
    response->setPayload(body, sizeof(body));

    channel->asyncSend(response->command2Buffer());
    return response;
}
