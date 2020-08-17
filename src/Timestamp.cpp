#include "Timestamp.h"
using namespace base;

#  define __PRI64_PREFIX	"l"
# define PRId64		__PRI64_PREFIX "d"

std::string base::Timestamp::toString() const
{
	char buf[32] = { 0 };
	int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%03" PRId64 "", seconds, microseconds);
	return buf;
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const
{
	char buf[64] = { 0 };
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
#ifdef  unix
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);
#else
    tm tm_time;
    gmtime_s(&tm_time,&seconds);
#endif
	if (showMicroseconds)
	{
		int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%3d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
			microseconds);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}
	return buf;
}

Timestamp Timestamp::now() {

        timeval tv{};
        gettimeofday(&tv, NULL);
        int64_t seconds = tv.tv_sec;
        return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);

}


#ifdef _WINDOWS
int Timestamp::gettimeofday(timeval * tp, void * tzp)
{
	time_t clock;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	struct tm tm = { wtm.wSecond, wtm.wMinute, wtm.wHour, wtm.wDay,
		wtm.wMonth - 1, wtm.wYear - 1900, wtm.wDayOfWeek, 0, 0 };

	clock = mktime(&tm)+(8*60*60);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);

}
#endif
