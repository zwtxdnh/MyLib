#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include "../include/CurrentThread.h"
#include"../include/Logger.h"
using namespace std;
int main()
{
    thread t([] {
        //cout<<this_thread::get_id()<<endl;
        LOG_INFO<< base::CurrentThread::tid();
        LOG_INFO<<syscall(SYS_gettid);
    });
    t.join();

    //cout<<this_thread::get_id()<<endl;
    LOG_INFO<< base::CurrentThread::tid();
    LOG_INFO<<syscall(SYS_gettid);
}