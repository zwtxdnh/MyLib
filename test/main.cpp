#include <iostream>

#include "../include/base/CurrentThread.h"
#include"../include/base/Logger.h"
#include"../include/base/noncopyable.h"
#include<atomic>
#ifdef unix
#include <unistd.h>
#include <sys/syscall.h>
#include<sys/epoll.h>
#include<sys/poll.h>
#endif

using namespace std;
using namespace base;


void CurrentThread_Test()

{
    thread t([] {
        //cout<<this_thread::get_id()<<endl;
        LOG_INFO<< base::CurrentThread::tid();
       // LOG_INFO<<syscall(SYS_gettid);
    });
    t.join();

    //cout<<this_thread::get_id()<<endl;
    LOG_INFO<< base::CurrentThread::tid();
    LOG_INFO<< base::CurrentThread::name();
    LOG_INFO<< base::CurrentThread::tidString();
    //LOG_INFO<<syscall(SYS_gettid);
}


class EventLoop:noncopyable
{
    EventLoop()
    :mLooping(false)
    {

    }
    ~EventLoop()
    = default;
    atomic <bool> mLooping;


};



void epoll_Test()
{



}

int main()
{

}