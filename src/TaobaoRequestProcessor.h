#pragma once
#include "RequestProcessor.h"


class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;
class TaobaoRequestProcessor : public RequestProcessor
{
public:
    TaobaoRequestProcessor(void);
    ~TaobaoRequestProcessor(void);
protected:
    RemotingCommandPtr process(ChannelPtr channel, RemotingCommandPtr request);
    AsyncTaskPtr procudeAsyncTask(ChannelPtr channel, RemotingCommandPtr request);
};
