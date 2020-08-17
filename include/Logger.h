#pragma once
#include"Timestamp.h"
#include"LogStream.h"
#undef ERROR

//20200712 05:35:20.651159Z  5127 INFO  **********runServer******** - main.cpp:136
namespace base {

	class Logger
	{
	public:
		enum LogLevel : uint8_t
		{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};

		class SourceFile
		{
		public:
			template<int N>
			SourceFile(const char(&arr)[N])
				: data_(arr),
				size_(N - 1)
			{
				const char* slash = strrchr(data_,'\\'); // 获取最后一个"\\"之后的字符串
				if (slash)
				{
					data_ = slash + 1;
					size_ -= static_cast<int>(data_ - arr);
				}
			}

			explicit SourceFile(const char* filename)//arg= __FILE__
				: data_(filename)
			{

				const char* slash = strrchr(filename, '\\');
				if (slash)
				{
					data_ = slash + 1;
				}
				size_ = static_cast<int>(strlen(data_));
			}

			const char* data_;
			int size_;
		};

		Logger(SourceFile file, int line);
		Logger(SourceFile file, int line, LogLevel level);
		Logger(SourceFile file, int line, LogLevel level, const char* func);
		Logger(SourceFile file, int line, bool toAbort);
		~Logger();

		//返回内部缓冲区中的日志流
		LogStream& stream() { return impl_.stream_; }

		static LogLevel logLevel();

		static void setLogLevel(LogLevel level);

		//输出函数 默认stdout(控制台)
		typedef void(*OutputFunc)(const char* msg, int len);

		//刷新函数 默认fflush
		typedef void(*FlushFunc)();

		//设置输出函数等........
		static void setOutput(OutputFunc);
		static void setFlush(FlushFunc);
		
	private:

		//logger内部缓冲区
		class Impl
		{
		public:
			typedef Logger::LogLevel LogLevel;
			Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
			//void formatTime();
			void finish();
			Timestamp time_;
			LogStream stream_;
			LogLevel level_;
			int line_;
			SourceFile basename_;
		};

		Impl impl_;

	};

	extern Logger::LogLevel g_logLevel;

	inline Logger::LogLevel Logger::logLevel()
	{
		return g_logLevel;
	}



#define LOG_TRACE if (base::Logger::logLevel() <= base::Logger::TRACE) \
  base::Logger(__FILE__, __LINE__, base::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (base::Logger::logLevel() <= base::Logger::DEBUG) \
  base::Logger(__FILE__, __LINE__, base::Logger::DEBUG, __func__).stream()
#define LOG_INFO /*if (base::Logger::logLevel() <= base::Logger::INFO)*/ \
 base::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN base::Logger(__FILE__, __LINE__, base::Logger::WARN).stream()
#define LOG_ERROR base::Logger(__FILE__, __LINE__, base::Logger::ERROR).stream()
#define LOG_FATAL base::Logger(__FILE__, __LINE__, base::Logger::FATAL).stream()
#define LOG_SYSERR base::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL base::Logger(__FILE__, __LINE__, true).stream()
}//namespace base

