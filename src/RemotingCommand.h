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
    * �����������룬��ͬ�������Ӧ��ͬ�ķ������߼�
    */
    string opCodeString;

    /**
    * �����ID, server��ͬ����ID��Ӧclient
    */
    size_t index;
    /**
    * �������ͷ�����Զ���kv��ʽ
    */
    map<string, string> header;
    /**
    * �����أ���Ϣ��
    */
    char* payload;
    size_t payloadLength;
    /**
    * ���󷵻��룬1��ʾ�ɹ���0��ʾʧ��
    */
    char resultCode;

    /**
    * ����ʱ�Ƿ��ͷ�payloadָ����ڴ�
    */
    bool releasePayload;
    /**
    * ���������
    */
    static AtomicCounter counter;
};
