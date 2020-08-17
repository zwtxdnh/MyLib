#include<iostream>
#include"../include/CurrentThread.h"
using namespace std;
int main()
{
    cout<<base::CurrentThread::tid()<<endl;
    cout<<"hello world";
}