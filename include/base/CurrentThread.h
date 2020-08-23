#pragma once
#include<thread>

#ifdef _WINDOWS
#include<Windows.h>
#include<TlHelp32.h>
#include<DbgHelp.h>
#else
#include <sys/syscall.h>
#include<unistd.h>
#endif


namespace base {

	namespace CurrentThread
	{
		extern thread_local int t_cachedTid;
		extern thread_local char t_tidString[32];
		extern thread_local int t_tidStringLength;
		extern thread_local const char* t_threadName;
		inline int tid()
		{
			if (t_cachedTid == 0)
			{
				#ifdef _WINDOWS
				auto tidobj=std::this_thread::get_id();
				t_cachedTid = *(int*)&tidobj;
				t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
				#else
				t_cachedTid=::syscall(SYS_gettid);
				#endif
			}
			return t_cachedTid;
		}

		inline char* tidString()
		{
			tid();
			return t_tidString;
		}
		inline int tidStringLength() 
		{
			tid();
			return t_tidStringLength;
		}
		inline const char* name()
		{
			return t_threadName;
		}

		bool isMainThread();
	}
	
}

