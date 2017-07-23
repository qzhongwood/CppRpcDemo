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
    * Ϊ��ͬ�Ĳ�����ע��ָ��������
    * @param opCode ���������
    * @param requestProcessor ��������
    * @param executorService �����̳߳ط������Ϊ�ձ�ʾ�ڵ�ǰ�̴߳��������Ϊ�գ���ô������̳߳���ִ����������
    */
    virtual void registerProcessor(string opCode, RequestProcessorPtr requestProcessor, ExecutorServicePtr executorService) = 0;
};
