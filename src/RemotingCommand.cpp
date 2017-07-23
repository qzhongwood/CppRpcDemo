#include "StdAfx.h"
#include "RemotingCommand.h"
#include "Buffer.h"

AtomicCounter RemotingCommand::counter;

RemotingCommand::RemotingCommand(void)
: resultCode(0)
, releasePayload(false)
{
    ++counter;
}

RemotingCommand::RemotingCommand(BufferPtr buffer)
: releasePayload(true)
{
    ++counter;
    buffer2Command(buffer);
}

RemotingCommand::~RemotingCommand(void)
{
    int n = --counter;
    rpcprintf("RemotingCommand destruction: <%x>, counter <%d>\n", this, n);
    if (releasePayload)
    {
        delete [] payload;
    }
}

string RemotingCommand::getOpCode() 
{
    return opCodeString;
}

void RemotingCommand::setOpCode(string opCodeString)
{
    this->opCodeString = opCodeString;
}

size_t RemotingCommand::getIndex() 
{
    return index;
}

void RemotingCommand::setIndex(size_t idx)
{
    this->index = idx;
}

map<string, string>& RemotingCommand::getHeader() 
{
    return header;
}

void RemotingCommand::setHeader(map<string, string> header)
{
    this->header = header;
}

void RemotingCommand::setPayload(char* payload, unsigned long payloadLength) 
{
    this->payload = payload;
    this->payloadLength = payloadLength;
}

char* RemotingCommand::getPayload() 
{
    return payload;
}

size_t RemotingCommand::getPayloadLength()
{
    return payloadLength;
}

char RemotingCommand::getResultCode()
{
    return resultCode;
}

void RemotingCommand::setResultCode(char resultCode)
{
    this->resultCode = resultCode;
}

void RemotingCommand::buffer2Command(BufferPtr source)
{
    source->setOffset(0);
    source->deserialize(opCodeString);
    source->deserialize((char*)&index, sizeof(index));
    source->deserialize((char*)&resultCode, sizeof(resultCode));
    size_t headerSize;
    source->deserialize((char*)&headerSize, sizeof(headerSize));

    for (size_t i = 0; i < headerSize; ++i)
    {
        string key;
        string value;
        source->deserialize(key);
        source->deserialize(value);
        header[key] = value;
    }
    source->deserialize((char*)&payloadLength, sizeof(payloadLength));
    releasePayload = true;
    payload = new char[payloadLength + 1];
    payload[payloadLength] = '\0';
    source->deserialize(payload, payloadLength);
}

BufferPtr RemotingCommand::command2Buffer()
{
    size_t headerSize = header.size();

    size_t length = sizeof(opCodeString.length());
    length += opCodeString.length();
    length += sizeof(index);
    length += sizeof(resultCode);
    length += sizeof(headerSize);

    map<string, string>::iterator it = header.begin();
    for (; it != header.end(); ++it)
    {
        length += sizeof(it->first.length());
        length += it->first.length();

        length += sizeof(it->second.length());
        length += it->second.length();
    }

    length += sizeof(payloadLength);
    length += payloadLength;


    BufferPtr buf = new Buffer(sizeof(length) + length);

    buf->serialize(length);
    buf->serialize(opCodeString);
    buf->serialize(index);
    buf->serialize((char*)&resultCode, sizeof(resultCode));
    buf->serialize(headerSize);

    it = header.begin();
    for (; it != header.end(); ++it)
    {
        buf->serialize(it->first);
        buf->serialize(it->second);
    }

    buf->serialize((char*)&payloadLength, sizeof(payloadLength));
    buf->serialize(payload, payloadLength);
    return buf;
}

string RemotingCommand::toString()
{
    char buf[10240];
    string headerString;
    map<string, string>::iterator it = header.begin();
    for (; it != header.end(); ++it)
    {
        char tmp[1024];

        if (headerString.length() > 0)
        {
            headerString += ",";
        }
        sprintf(tmp, "<%s>=<%s>", it->first.c_str(), it->second.c_str());
        headerString += tmp;
    }

    sprintf(buf, "index<%d>, opcode<%s>, resultCode<%d>, header<%s>, payload_length<%d>, payload<%s>",
        index,
        opCodeString.c_str(),
        resultCode,
        headerString.c_str(),
        payloadLength,
        payload);

    return buf;
}
