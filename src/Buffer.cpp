#pragma once

#include "Buffer.h"

AtomicCounter Buffer::counter;
Buffer::Buffer(size_t len)
: releaseBuf(true)
, length(len)
, offset(0)
{
    ++counter;
    //printf("new Buffer from remote\n");
    data = new char[length + 1];
    memset(data, 0, length + 1);
}

Buffer::Buffer(char* b, size_t len)
: releaseBuf(false)
, data(b)
, length(len)
, offset(len)
{
    ++counter;
    //printf("new Buffer local\n");
}

Buffer::~Buffer(void)
{
    int n = --counter;
    printf("~Buffer: <%x>, counter <%d>\n", this, n);
  
    if (releaseBuf)
    {
        delete [] data;
    }
}

void Buffer::setOffset(size_t off)
{
    offset = 0;
}

int Buffer::serialize(size_t context)
{
    serialize((char*)&context, sizeof(context));
    return 0;
}


int Buffer::serialize(const string& str)
{
    size_t len = str.length();
    serialize((char*)&len, sizeof(len));

    serialize(const_cast<char*>(str.c_str()), len);
    return 0;

}

int Buffer::serialize(char* context, size_t len)
{
    if (offset <= length - len)
    {
        memcpy(data + offset, context, len);
        offset += len;
    }
    return 0;
}

void Buffer::setNext(BufferPtr next)
{
    this->next = next;
    next->prev = this;
}

void Buffer::clear()
{
    if (next != NULL) 
    {
        next->prev = NULL;
        next = NULL;
    }

    if (prev != NULL) 
    {
        prev->next = NULL;
        prev = NULL;
    }
}

BufferPtr Buffer::nextBuffer()
{
    return next;
}

BufferPtr Buffer::prevBuffer()
{
    return prev;
}

int Buffer::deserialize(char* context, size_t len)
{
    if (offset <= length - len)
    {
        memcpy(context, data + offset, len);
        offset += len;
    }

    return 0;
}

int Buffer::deserialize(string& context)
{
    size_t len;
    deserialize((char*)&len, sizeof(len));

    char* buf = new char[len + 1];
    buf[len] = '\0';
    deserialize(buf, len);
    context = buf;
    delete [] buf;
    return 0;
}

char* Buffer::getBuf()
{
    return data;
}

size_t Buffer::getLength()
{
    return length;
}