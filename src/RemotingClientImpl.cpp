#include "StdAfx.h"
#include <time.h> 
#include <WINSOCK2.H>
#include "RemotingClientImpl.h"
#include "RequestProcessor.h"
#include "ExecutorService.h"
#include "RemotingCommand.h"
#include "Channel.h"
#include "Channel.h"
#include "Connector.h"
#include "NetworkIoEngine.h"
#include "Buffer.h"
#include "Lock.h"
#include "RemotingServer.h"
#include "Exception.h"

RemotingClientImpl::RemotingClientImpl(void)
{
}

RemotingClientImpl::~RemotingClientImpl(void)
{
}

void RemotingClientImpl::start()
{
}

ChannelPtr RemotingClientImpl::getChannel(string address, int port)
{
    char key[32];
    sprintf(key, "%s:%d", address.c_str(), port);
    ChannelPtr channel;
    {
        Lock lock(channelMapMutex);
        map<string, ChannelPtr>::iterator it = channelMap.begin();

        if (it != channelMap.end())
        {
            channel = it->second;
        }
        else
        {
            channel = Connector::connect(address, port);
            if (channel != NULL)
            {
                channelMap[key] = channel;
                channel->setRemotingCommandHandler(this);
            }
        }
    }
    return channel;
}

void RemotingClientImpl::stop()
{
    Lock lock(channelMapMutex);
    map<string, ChannelPtr>::iterator it = channelMap.begin();
    for (; it != channelMap.end(); ++it)
    {
        ChannelPtr channel = it->second;
        channel->shutdown();
    }
    channelMap.clear();
}

RemotingCommandPtr RemotingClientImpl::invoke(string address, int port, RemotingCommandPtr command, long timeOut)
{
    RemotingCommandPtr response;
    if (command == NULL)
    {
        return response;
    }
    
    size_t index = command->getIndex();
    EventPtr event = new Event(true);
    {
        Lock lock(eventMapMutex);
        eventMap[index] = event;
    }

    submit(address, port, command);
    Sleep(1);

    event->await(timeOut);

    {
        Lock lock(eventMapMutex);
        map<size_t, EventPtr>::iterator it = eventMap.find(index);
        if (it != eventMap.end())
        {
            eventMap.erase(it);
        }
        else
        {
            map<size_t, RemotingCommandPtr>::iterator it = responseMap.find(index);
            if (it != responseMap.end())
            {
                response = it->second;
            }
        }       
    }
    return response;
}


void RemotingClientImpl::submit(string address, int port, RemotingCommandPtr command)
{
    ChannelPtr channel = getChannel(address, port);
    if (channel != NULL)
    {
        BufferPtr buf = command->command2Buffer();
        channel->asyncSend(buf);
    }
    else
    {
        throw new Exception("Failed to build connection!");
    }
}


void RemotingClientImpl::start(string address, int port)
{
    currentChannel = getChannel(address, port);
}

RemotingCommandPtr RemotingClientImpl::asyncInvoke(RemotingCommandPtr command)
{
    if (currentChannel != NULL)
    {
        BufferPtr buf = command->command2Buffer();
        currentChannel->asyncSend(buf);
    }
    return NULL;
}

void RemotingClientImpl::onRemotingCommand(ChannelPtr channel, BufferPtr responseBuffer)
{
    RemotingCommandPtr response = new RemotingCommand(responseBuffer);
    rpcprintf("onRemotingCommand happened!\n");
    {
        Lock lock(eventMapMutex);
        map<size_t, EventPtr>::iterator it = eventMap.find(response->getIndex());
        if (it != eventMap.end())
        {
            rpcprintf("signal happened!\n");
            (it->second)->signal();
            eventMap.erase(it);
        }
        responseMap[response->getIndex()] = response;
    }
}

void RemotingClientImpl::fetchResponse(list<RemotingCommandPtr>& cmdList)
{
    Lock lock(eventMapMutex);
    map<size_t, RemotingCommandPtr>::iterator it = responseMap.begin();

    for(; it != responseMap.end(); ++it)
    {
        cmdList.push_back(it->second);
    }
    responseMap.clear();    
}

