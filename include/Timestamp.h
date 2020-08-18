#pragma once

#include<string>

#include <ctime>
#ifdef _WINDOWS
#include<Windows.h>
#else
#include<sys/time.h>
#endif
/*
�ڲ����浱����߹���΢������ptime
*/

namespace base {

	class Timestamp
	{

	public:

		Timestamp() :microSecondsSinceEpoch_(0) {}


		explicit Timestamp(int64_t microSecondsSinceToday)
			: microSecondsSinceEpoch_(microSecondsSinceToday) {}


		void swap(Timestamp that)
		{
			std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
		}

		bool vaild() const { return microSecondsSinceEpoch_ > 0; }



		inline bool operator==(Timestamp rhs)
		{

			return this->microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
		}

		inline bool operator<(Timestamp rhs)
		{
			return this->microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
		}
		inline int64_t getMicroSecond()
		{
			return this->microSecondsSinceEpoch_;
		}

		std::string toString() const;

		std::string toFormattedString(bool showMicroseconds = true) const;


		inline Timestamp operator-(Timestamp& rhs)
		{
			return Timestamp(this->microSecondsSinceEpoch_ - rhs.microSecondsSinceEpoch_);
		}

		static Timestamp now();



		int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

		time_t secondsSinceEpoch() const
		{
			return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
		}

#ifdef _WINDOWS
		static int gettimeofday(struct timeval *tp, void *tzp = nullptr);
#endif
		static const int kMicroSecondsPerSecond = 1000 * 1000;
	private:
		int64_t microSecondsSinceEpoch_;

	};

	inline bool operator<(Timestamp lhs, Timestamp rhs)
	{
		return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
	}

	inline bool operator==(Timestamp lhs, Timestamp rhs)
	{
		return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
	}


	inline Timestamp addTime(Timestamp timestamp, double seconds)
	{
		int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
		return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
	}


	inline double timeDifference(Timestamp high, Timestamp low)
	{
		int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
		return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
	}

}//namespace base
