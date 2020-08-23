#include "base/ThreadPool.h"

namespace base
{
	ThreadPool::ThreadPool(const std::string & nameArg)
		:mutex_(),
		running_(false),
		maxQueueSize_(0),
		name_(nameArg),
		numOfWorking_(0)
	{
	}

	ThreadPool::~ThreadPool()
	{
		if (running_)
			stop();
	}

	void ThreadPool::start(int numThreads)
	{
		assert(threads_.empty());
		//设置runing标志位
		running_ = true;
		//设置vector大小
		threads_.reserve(numThreads);
		//执行最大线程数量的线程
		for (int i = 0; i < numThreads; i++)
		{
			//向线程容器中加入一个线程,线程函数
			threads_.emplace_back(new std::thread(std::bind(&ThreadPool::runInThread, this)));
			//threads_[i]
		}
		//若线程池大小为0且存在线程初始化回调,那么执行线程初始化回调
		if (numThreads == 0 && threadInitCallback_)
		{
			threadInitCallback_();
		}
	}

	void ThreadPool::stop()
	{
		{
			std::unique_lock<std::mutex> lock(mutex_);
			running_ = false;
			notEmpty_.notify_all();
		}
		for (auto& t : threads_)
		{
			t->join();
		}
	}
	//等待所有task结束,若等待超时就返回false
	bool ThreadPool::waitForAllDone(uint32_t millisecond)
	{

		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (millisecond)
			{
				if (allDone_.wait_for(lock, std::chrono::milliseconds(millisecond)) == std::cv_status::timeout)
				{
					return false;
				}
			}
			else
			{
				allDone_.wait(lock);
			}

		}
		//while (!queue_.empty())
		//{
		//	std::this_thread::yield();
		//}
		stop();
		return true;
	}

	void ThreadPool::run(Task task)
	{
		if (threads_.empty())
		{
			task();
		}
		else
		{
			std::unique_lock<std::mutex> lock(mutex_);
			while (isFull())
			{
				notFull_.wait(lock);
			}
			assert(!isFull());
			queue_.push_back(std::move(task));
			notEmpty_.notify_one();
		}
	}

	bool ThreadPool::isFull() const
	{
		return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
	}

	ThreadPool::Task ThreadPool::take()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		while (queue_.empty() && running_)
		{
			notEmpty_.wait(lock);
		}
		Task task;

		if (!queue_.empty())
		{
			//取出任务队列第一个元素
			task = queue_.front();
			queue_.pop_front();

			//if (queue_.empty()&&threads_.)
			//	Empty_.notify_one();
			//队列中有空位了,就进行通知
			if (maxQueueSize_ > 0)
			{
				notFull_.notify_one();
			}
		}
		//返回线程执行函数
		return task;
	}
	void ThreadPool::runInThread()
	{
		try
		{
			//若有线程初始化函数就执行之
			if (threadInitCallback_)
			{
				threadInitCallback_();
			}
			//从任务队列中取一个任务执行
			while (running_)
			{
				Task task(take());

				if (task)
				{
					//执行任务
					numOfWorking_++;
					task();
					numOfWorking_--;
					std::unique_lock<std::mutex> lock(mutex_);
					if (numOfWorking_ == 0 && queue_.empty())
						allDone_.notify_one();

				}

			}

		}
		//catch (const std::exception& ex)
		//{
		//	fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
		//	fprintf(stderr, "reason: %s\n", ex.what());
		//	//fprintf(stderr, "stack trace: %s\n", ex.what));
		//	abort();
		//}
		catch (const std::exception& ex)
		{
			fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		}
		catch (...)
		{
			fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
			throw; // rethrow
		}
	}




}