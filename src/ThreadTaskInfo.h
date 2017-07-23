#pragma once
#include "Referable.h"

class AsyncTask : public Referable
{
public:
    AsyncTask() 
    {}

    virtual ~AsyncTask()
    {}

    void SafeExecute() 
    {
        try
        {
            Execute();
        }
        catch (...)
        {
        }
    }

protected:
    virtual void Execute()=0;
};

typedef Ref<AsyncTask> AsyncTaskPtr;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// 2 params
template <typename Obj, typename MemFun, typename P1, typename P2>
class AsyncTaskImpl : public AsyncTask
{
public:
    static AsyncTaskPtr ThreadStartInfo(Obj* obj, MemFun function, P1 p, P2 p2)
    {
        AsyncTaskImpl<Obj, MemFun,P1, P2>* task = 
            new AsyncTaskImpl<Obj, MemFun, P1, P2>(obj, function, p, p2);
        return AsyncTaskPtr(task);
    }

protected:
    AsyncTaskImpl(Obj* obj, MemFun function, P1 p1, P2 p2) 
        : obj_(obj), memberFunction_(function), p1_(p1), p2_(p2) {}

    virtual void Execute() 
    {
        (obj_->*memberFunction_)(p1_,p2_);
    }

    Obj* obj_;
    MemFun memberFunction_;
    const P1 p1_;
    const P2 p2_;
};

template <typename Obj, typename MemFun, typename P1, typename P2>
inline AsyncTaskPtr ThreadStartInfo(Obj* obj, MemFun function, P1 p1, P2 p2)
{
    return AsyncTaskImpl<Obj, MemFun, P1, P2>::ThreadStartInfo(obj, function, p1, p2);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// 3 params
template <typename Obj, typename MemFun, typename P1, typename P2, typename P3>
class AsyncTaskImpl3 : public AsyncTask
{
public:
    static AsyncTaskPtr ThreadStartInfo(Obj* obj, MemFun function, P1 p, P2 p2, P3 p3)
    {
        AsyncTaskImpl3<Obj, MemFun, P1, P2, P3>* task = 
            new AsyncTaskImpl3<Obj, MemFun, P1, P2, P3>(obj, function, p, p2, p3);
        return AsyncTaskPtr(task);
    }

protected:
    AsyncTaskImpl3(Obj* obj, MemFun function, P1 p1, P2 p2, P3 p3) 
        : obj_(obj), memberFunction_(function), p1_(p1), p2_(p2), p3_(p3) {}

    virtual void Execute() 
    {
        (obj_->*memberFunction_)(p1_, p2_, p3_);
    }

    Obj* obj_;
    MemFun memberFunction_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
};

template <typename Obj, typename MemFun, typename P1, typename P2, typename P3>
inline AsyncTaskPtr ThreadStartInfo(Obj* obj, MemFun function, P1 p1, P2 p2, P3 p3)
{
    return AsyncTaskImpl3<Obj, MemFun, P1, P2, P3>::ThreadStartInfo(obj, function, p1, p2, p3);
}
