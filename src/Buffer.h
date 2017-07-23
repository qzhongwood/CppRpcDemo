#pragma once
#include "string"
#include "Referable.h"
using namespace std;

class Buffer;
typedef Ref<Buffer> BufferPtr;

class Buffer : public Referable
{
public:
    Buffer(size_t len);
    Buffer(char* b, size_t len);
    ~Buffer(void);

    char* getBuf();
    size_t getLength();

    void setOffset(size_t off);

    int serialize(const string& str);
    int serialize(size_t context);
    int serialize(char* context, size_t len);

    int deserialize(char* context, size_t len);
    int deserialize(string& context);

    BufferPtr nextBuffer();
    BufferPtr prevBuffer();
    void setNext(BufferPtr next);
    void clear();
private:
    void setPrev(BufferPtr next);
    char* data;
    size_t length;
    size_t offset;
    bool releaseBuf;

    BufferPtr next;
    BufferPtr prev;
    static AtomicCounter counter;

};

