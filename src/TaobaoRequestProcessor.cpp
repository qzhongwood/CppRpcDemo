#include "StdAfx.h"
#include "TaobaoRequestProcessor.h"
#include "RemotingCommand.h"
#include "Channel.h"
#include "Buffer.h"
#include "RemotingCommandHandler.h"
#include "RemotingServer.h"
#include "ExecutorService.h"


TaobaoRequestProcessor::TaobaoRequestProcessor(void)
{
}

TaobaoRequestProcessor::~TaobaoRequestProcessor(void)
{
}

AsyncTaskPtr TaobaoRequestProcessor::procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request)
{
    return ThreadStartInfo(
        this, &TaobaoRequestProcessor::process, 
        channel, request);
}


RemotingCommandPtr TaobaoRequestProcessor::process(ChannelPtr channel, RemotingCommandPtr request)
{
    RemotingCommandPtr response = new RemotingCommand();
    response->setOpCode(request->getOpCode());
    response->setResultCode(1);
    response->setIndex(request->getIndex());

    char buf[256];
    map<string, string> headerMap = request->getHeader();
    map<string, string>::iterator it = headerMap.find("city");
    if (it != headerMap.end())
    {
        sprintf(buf, "%s is in %s.", request->getPayload(), it->second.c_str());
    }

    rpcprintf("Taobao processor working. Thread Id <%d>. Result <%s>\n", GetCurrentThreadId(), buf);
    response->setPayload(buf, sizeof(buf));
    channel->asyncSend(response->command2Buffer());
    return response;
}
