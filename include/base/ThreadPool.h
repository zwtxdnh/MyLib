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

		//��������߳���
		void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
		//�����̳߳�ʼ���ص�
		void setThreadInitCallback(const Task& cb)
		{
			threadInitCallback_ = cb;
		}
		//���������߳�
		void start(int numThreads);
		//���������߳�
		void stop();
		//��ȡ�̳߳ص�����
		const std::string& name() const
		{
			return name_;
		}
		//��ȡ�̺߳������еĴ�С
		size_t queueSize() const 
		{
			std::unique_lock<std::mutex> lock(mutex_);
			return queue_.size();
		}

		//�ȴ������������
		bool waitForAllDone(uint32_t millisecond=0);

		//���̳߳������һ������
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
