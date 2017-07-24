#pragma once
#include <string>
#include <map>
#include "Referable.h"

class Buffer;
typedef Ref<Buffer> BufferPtr;

class RemotingCommand;
typedef Ref<RemotingCommand> RemotingCommandPtr;


using namespace std;

class RemotingCommand : public Referable
{
public:
    RemotingCommand(void);
    ~RemotingCommand(void);
    RemotingCommand(BufferPtr source);

    string toString();

    string getOpCode();
    void setOpCode(string opCode);

    size_t getIndex();
    void setIndex(size_t idx);

    map<string, string>& getHeader();
    void setHeader(const map<string, string>& header);

    char* getPayload();
    size_t getPayloadLength();
    void setPayload(char* payload, unsigned long payloadLength);

    char getResultCode();
    void setResultCode(char resultCode);

    BufferPtr command2Buffer();

protected:
    void buffer2Command(BufferPtr source);

private:
    /**
    * 请求对象操作码，不同操作码对应不同的服务处理逻辑
    */
    string opCodeString;

    /**
    * 请求的ID, server用同样的ID响应client
    */
    size_t index;
    /**
    * 请求对象头部，自定义kv格式
    */
    map<string, string> header;
    /**
    * 请求负载，消息体
    */
    char* payload;
    size_t payloadLength;
    /**
    * 请求返回码，1表示成功，0表示失败
    */
    char resultCode;

    /**
    * 析构时是否释放payload指向的内存
    */
    bool releasePayload;
    /**
    * 请求计数器
    */
    static AtomicCounter counter;
};
