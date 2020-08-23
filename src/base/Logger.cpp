#include "base/Logger.h"
#include "base/CurrentThread.h"
#pragma warning(disable:4996)
namespace base
{

	void defaultOutput(const char* msg, int len)
	{
		size_t n = fwrite(msg, 1, len, stdout);
		//FIXME check n
		(void)n;
	}

	void defaultFlush()
	{
		fflush(stdout);
	}

	Logger::OutputFunc g_output = defaultOutput;
	Logger::FlushFunc g_flush = defaultFlush;
	Logger::LogLevel g_logLevel = Logger::INFO;

	const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
	{
	  "TRACE ",
	  "DEBUG ",
	  "INFO  ",
	  "WARN  ",
	  "ERROR ",
	  "FATAL ",
	};


	class T
	{
	public:
		T(const char* str, unsigned len)
			:str_(str),
			len_(len)
		{
			assert(strlen(str) == len_);
		}

		const char* str_;
		const unsigned len_;
	};



	inline LogStream& operator<<(LogStream& s, T v)
	{
		s.append(v.str_, v.len_);
		return s;
	}


	inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
	{
		s.append(v.data_, v.size_);
		return s;
	}

	//��־��ʽ
	Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
		: time_(Timestamp::now()),
		stream_(),
		level_(level),
		line_(line),
		basename_(file)
	{
		stream_ << T(time_.toFormattedString().c_str(), strlen(time_.toFormattedString().c_str()));
		stream_ << " " << CurrentThread::tid() << "  ";
		stream_ << T(LogLevelName[level], 6);
	}



	Logger::Logger(SourceFile file, int line)
		: impl_(INFO, 0, file, line)
	{
	}

	Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
		: impl_(level, 0, file, line)
	{
		impl_.stream_ << func << ' ';
	}

	Logger::Logger(SourceFile file, int line, LogLevel level)
		: impl_(level, 0, file, line)
	{
	}

	Logger::Logger(SourceFile file, int line, bool toAbort)
		: impl_(toAbort ? FATAL : ERROR, errno, file, line)
	{
	}

	Logger::~Logger()
	{
		impl_.finish();
		const LogStream::Buffer& buf(stream().buffer());
		g_output(buf.data(), buf.length());
		if (impl_.level_ == FATAL)
		{
			g_flush();
			abort();
		}
	}


	void Logger::Impl::finish()
	{
		stream_ << " - " << basename_ << ':' << line_ << '\n';
	}


	void Logger::setLogLevel(Logger::LogLevel level)
	{
		g_logLevel = level;
	}

	void Logger::setOutput(OutputFunc out)
	{
		g_output = out;
	}

	void Logger::setFlush(FlushFunc flush)
	{
		g_flush = flush;
	}



 }


