#include"Thread.h"
#include"CurrentThread.h"

#include<assert.h>
#include<errno.h>
#include<linux/unistd.h>
#include<stdint.h>
#include<stdio.h>
#include<sys/prctl.h>
#include<sys/types.h>
#include<unistd.h>
#include<memory>
#include<iostream>

namespace ccy{

namespace CurrentThread{
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "default";
}

pid_t getTid() {return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cachedTid(){
    if(t_cachedTid == 0){
        t_cachedTid = getTid();
        t_tidStringLength = 
            snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
    }
}

class ThreadData{
public:
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadData(const ThreadFunc& func, const std::string& name,
                pid_t* tid, CountDownLatch* latch)
    : _func(func), _name(name), _tid(tid), _latch(latch){}

    void runInThread(){
        *_tid = CurrentThread::tid();
        _tid = nullptr;
        _latch->countDown();
        _latch = nullptr;

        CurrentThread::t_threadName = _name.empty()? "Thread": _name.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);

        _func();
        CurrentThread::t_threadName = "finished";
    }

private:
    ThreadFunc _func;
    std::string _name;
    pid_t* _tid;
    CountDownLatch* _latch;
};

void* startThread(void* obj){
    std::unique_ptr<ThreadData> data(static_cast<ThreadData*>(obj));
    data->runInThread();
    return nullptr;
}

Thread::Thread(const ThreadFunc& func, const std::string& n)
    : _started(false)
    , _joined(false)
    , _pthreadId(0)
    ,_tid(0)
    ,_func(func)
    ,_name(n)
    ,_latch(1)
{
    setDefaultName();
}

Thread::~Thread(){
    if(_started && !_joined) pthread_detach(_pthreadId);
}





}
