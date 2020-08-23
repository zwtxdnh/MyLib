#ifdef unix
#include <unistd.h>
#endif
#include "base/CurrentThread.h"
namespace base
{
	namespace CurrentThread
	{

        thread_local int t_cachedTid = 0;
        thread_local char t_tidString[32];
        thread_local int t_tidStringLength = 6;
        thread_local const char* t_threadName = "unknown";
#ifdef _WINDOWS
		DWORD GetMainThreadId(DWORD processId = 0)
		{
			if (processId == 0)
				processId = GetCurrentProcessId();

			DWORD threadId = 0;
			THREADENTRY32 te32 = { sizeof(te32) };
			HANDLE threadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			if (Thread32First(threadSnap, &te32))
			{
				do
				{
					if (processId == te32.th32OwnerProcessID)
					{
						threadId = te32.th32ThreadID;
						break;
					}
				} while (Thread32Next(threadSnap, &te32));
			}
			return threadId;
		}
#endif // _WIN64

		bool isMainThread()
		{
#ifdef _WINDOWS
			return tid() == GetMainThreadId();
#else
			return tid() == getpid();
#endif

			
		}
	}
}