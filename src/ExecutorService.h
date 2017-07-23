#pragma once
#include "Referable.h"
#include <list>
#include "ThreadTaskInfo.h"
#include "Event.h"
#include "Mutex.h"

using namespace std;

class ExecutorService;
typedef Ref<ExecutorService> ExecutorServicePtr;

class ExecutorService : public Referable
{
public:
    ExecutorService(const string& name, int numThread = 3);
    ~ExecutorService(void);

    void spawn();
    void submitTask(AsyncTaskPtr task);

    bool isStopped();

    static DWORD WINAPI TaskThread(LPVOID IpParam);

protected:
    string name;
    bool stop;
    int numThread;
    int numTaskExecuted;
    Event event;
    Mutex mutex;
    list<AsyncTaskPtr> runnableTaskList;
};
