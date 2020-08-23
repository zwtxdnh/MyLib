#pragma once
#include"noncopyable.h"
#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<deque>
#include<assert.h>
#include<atomic>
#include <functional>

namespace base
{
	class ThreadPool :noncopyable
	{
	public:
		typedef std::function<void()> Task;
		explicit ThreadPool(const std::string& nameArg = std::string("ThreadPool"));
		~ThreadPool();

		//设置最大线程数
		void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
		//设置线程初始化回调
		void setThreadInitCallback(const Task& cb)
		{
			threadInitCallback_ = cb;
		}
		//启动所有线程
		void start(int numThreads);
		//结束所有线程
		void stop();
		//获取线程池的名字
		const std::string& name() const
		{
			return name_;
		}
		//获取线程函数队列的大小
		size_t queueSize() const 
		{
			std::unique_lock<std::mutex> lock(mutex_);
			return queue_.size();
		}

		//等待所有任务结束
		bool waitForAllDone(uint32_t millisecond=0);

		//向线程池中添加一个任务
		void run(Task f);

	private:
		bool isFull() const;
		void runInThread();
		Task take();
		mutable std::mutex mutex_;
		std::condition_variable allDone_, notEmpty_, notFull_;
		std::string name_;
		Task threadInitCallback_;
		std::vector<std::unique_ptr<std::thread>> threads_;
		std::deque<Task> queue_;
		size_t maxQueueSize_;
		bool running_;
		std::atomic_int numOfWorking_;
	};

}
