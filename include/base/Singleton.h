#pragma once
#include"noncopyable.h"
#include<assert.h>
#include<mutex>
namespace base
{
	//template <typename C> static T32_t test(...);
	
	template <typename T>
	
	class Singleton: noncopyable
	{
	public:
		static T & instance()
		{
			std::call_once(flag_, &Singleton::init);
			assert(value_ != nullptr);
			return *value_;
		}
		~Singleton()
		{
			delete value_;
		}
	private:

		static void init()
		{
			value_ = new T();
		}
		static std::once_flag flag_;
		static T* value_;
	};

	template<typename T>
	std::once_flag Singleton<T>::flag_ ;

	template<typename T>
	T* Singleton<T>::value_ = nullptr;
	
}

