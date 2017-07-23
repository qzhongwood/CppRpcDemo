#pragma once
#include "RequestProcessor.h"

class AntRequestProcessor : public RequestProcessor
{
public:
    AntRequestProcessor(void);
    ~AntRequestProcessor(void);
protected:
    RemotingCommandPtr process(ChannelPtr channel, RemotingCommandPtr request);
    AsyncTaskPtr procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request);
};
