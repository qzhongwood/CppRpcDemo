#pragma once
#include "RequestProcessor.h"

class TmallRequestProcessor : public RequestProcessor
{
public:
    TmallRequestProcessor(void);
    ~TmallRequestProcessor(void);
protected:
    RemotingCommandPtr process(ChannelPtr channel, RemotingCommandPtr request);
    AsyncTaskPtr procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request);
};
