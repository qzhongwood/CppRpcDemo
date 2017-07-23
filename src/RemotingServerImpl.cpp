
#include "StdAfx.h"
#include "RemotingServerImpl.h"
#include "ExecutorService.h"
#include "RequestProcessor.h"
#include "TmallRequestProcessor.h"
#include "TaobaoRequestProcessor.h"
#include "AntRequestProcessor.h"
#include "RemotingCommand.h"
#include "Buffer.h"
#include "Channel.h"
#include "Acceptor.h"

RemotingServerImpl::RemotingServerImpl(void)
{
    
}

RemotingServerImpl::~RemotingServerImpl(void)
{
}


void RemotingServerImpl::start(int port)
{
    ExecutorServicePtr taobaoPool = new ExecutorService("Taobao processor");
    ExecutorServicePtr tmallPool  = new ExecutorService("Tmall processor");
    ExecutorServicePtr antPool    = new ExecutorService("Ant processor");

    registerProcessor("1", new TaobaoRequestProcessor(), taobaoPool);
    registerProcessor("2", new TmallRequestProcessor(), tmallPool);
    registerProcessor("3", new AntRequestProcessor(), antPool);

    Acceptor acceptor;
    acceptor.setRemotingCommandHandler(this);
    acceptor.start(port);
}


void RemotingServerImpl::registerProcessor(string opCode, 
                                           RequestProcessorPtr requestProcessor, 
                                           ExecutorServicePtr executorService)
{
    processorMap[opCode] = requestProcessor;
    requestProcessor->setThreadPool(executorService);
}

void RemotingServerImpl::onRemotingCommand(ChannelPtr channel, RemotingCommandPtr cmd)
{
    map<string, RequestProcessorPtr>::iterator it = processorMap.find(cmd->getOpCode());
    if (it != processorMap.end())
    {
        RequestProcessorPtr processor = it->second;
        processor->processRequest(channel, cmd);
    }
    else
    {
        printf("Unregistered processor: command opcode<%s>", cmd->getOpCode().c_str());
    }
}

void RemotingServerImpl::onRemotingCommand(ChannelPtr channel, BufferPtr buffer)
{
    RemotingCommandPtr cmd = new RemotingCommand(buffer);
    //printf("Server: command<%s>", cmd->toString().c_str());
    onRemotingCommand(channel, cmd);
}

