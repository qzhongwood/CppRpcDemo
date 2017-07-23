#pragma once
#include "string"
#include "Referable.h"
#include "RemotingCommandHandler.h"

class RequestProcessor;
typedef Ref<RequestProcessor> RequestProcessorPtr;

class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;

class Channel;
typedef Ref<Channel> ChannelPtr;

class Buffer;
typedef Ref<Buffer> BufferPtr;

using namespace std;

class RemotingServer : public RemotingCommandHandler
{
public:
    virtual void start(int port) = 0;
    /**
    * 为不同的操作码注册指定处理器
    * @param opCode 请求操作码
    * @param requestProcessor 请求处理器
    * @param executorService 处理线程池服务，如果为空表示在当前线程处理；如果不为空，那么在这个线程池里执行请求处理器
    */
    virtual void registerProcessor(string opCode, RequestProcessorPtr requestProcessor, ExecutorServicePtr executorService) = 0;
};
