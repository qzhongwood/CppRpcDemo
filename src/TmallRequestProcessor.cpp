#include "StdAfx.h"
#include "TmallRequestProcessor.h"
#include "RemotingCommand.h"
#include "Channel.h"
#include "RemotingCommandHandler.h"
#include "Buffer.h"
#include "ExecutorService.h"

TmallRequestProcessor::TmallRequestProcessor(void)
{
}

TmallRequestProcessor::~TmallRequestProcessor(void)
{
}


AsyncTaskPtr TmallRequestProcessor::procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request)
{
    return ThreadStartInfo(
        this, &TmallRequestProcessor::process, 
        channel, request);
}

RemotingCommandPtr TmallRequestProcessor::process(ChannelPtr channel, RemotingCommandPtr request)
{
    RemotingCommandPtr response = new RemotingCommand();
    response->setOpCode(request->getOpCode());
    response->setResultCode(0);
    response->setIndex(request->getIndex());

    printf("Tmall processor working. request id<%d>, Thread Id %d\n",
        request->getIndex(),
        GetCurrentThreadId());
    Sleep(10000);

    char body[] = "This is from TmallRequestProcessor";
    response->setPayload(body, sizeof(body));

    channel->asyncSend(response->command2Buffer());
    return response;
}
