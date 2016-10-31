
#define _CRT_SECURE_NO_WARNINGS

#include <GameLibrary/Utilities/Time/DateTime.h>
#include <GameLibrary/Utilities/ArrayList.h>
#include <GameLibrary/Utilities/Math.h>
#include <GameLibrary/Utilities/PlatformChecks.h>
#include "posixtime.h"
#include "time64/time64.h"
#include <ctime>

#ifndef TARGETPLATFORM_WINDOWS
	#include <sys/time.h>
#endif

#ifdef min
#undef min
#endif

namespace fgl
{
	#define DATETIME_TM_BASEYEAR 1900
	
	bool DateTime_checkLeapYear(int year)
	{
		if((year % 4)==0 && ((year % 100)!=0 || (year % 400)==0))
		{
			return true;
		}
		return false;
	}
	
	int DateTime::getLocalUTCOffset()
	{
		struct timeval64 tv;
		gettimeofday64(&tv, nullptr);
		Time64_T now = (Time64_T)tv.tv_sec;
		struct tm lcl = *localtime64(&now);
		struct tm gmt = *gmtime64(&now);
		
		Time64_T lcl_total = mktime64(&lcl);
		Time64_T gmt_total = mktime64(&gmt);
		return (int)(((Int64)lcl_total - (Int64)gmt_total)/60);
	}
	
	DateTime::DateTime()
	{
		struct timeval64 tv;
		gettimeofday64(&tv, nullptr);
		Time64_T now = (Time64_T)tv.tv_sec;
		struct tm lcl = *localtime64(&now);
		usec = (int)tv.tv_usec;
		utc_offset = DateTime::getLocalUTCOffset();
		sec = lcl.tm_sec;
		min = lcl.tm_min;
		hour = lcl.tm_hour;
		mday = lcl.tm_mday;
		mon = lcl.tm_mon + 1;
		year = lcl.tm_year + DATETIME_TM_BASEYEAR;
		wday = lcl.tm_wday + 1;
		yday = lcl.tm_yday + 1;
	}
	
	DateTime::DateTime(const DateTime& datetime)
		: utc_offset(datetime.utc_offset),
		usec(datetime.usec),
		sec(datetime.sec),
		min(datetime.min),
		hour(datetime.hour),
		mday(datetime.mday),
		mon(datetime.mon),
		year(datetime.year),
		wday(datetime.wday),
		yday(datetime.yday)

	{
		//
	}
	
	DateTime::DateTime(const time_t& timeval)
	{
		assign(timeval);
	}

	DateTime::~DateTime()
	{
		//
	}
	
	void DateTime::assign(const DateTime& datetime)
	{
		utc_offset = datetime.utc_offset;
		usec = datetime.usec;
		sec = datetime.sec;
		min = datetime.min;
		hour = datetime.hour;
		mday = datetime.mday;
		mon = datetime.mon;
		year = datetime.year;
		wday = datetime.wday;
		yday = datetime.yday;
	}
	
	void DateTime::assign(const time_t& timeval)
	{
		struct tm lcl = *localtime(&timeval);
		utc_offset = DateTime::getLocalUTCOffset();
		usec = 0;
		sec = lcl.tm_sec;
		min = lcl.tm_min;
		hour = lcl.tm_hour;
		mday = lcl.tm_mday;
		mon = lcl.tm_mon + 1;
		year = lcl.tm_year + DATETIME_TM_BASEYEAR;
		wday = lcl.tm_wday + 1;
		yday = lcl.tm_yday + 1;
	}
	
	int DateTime::getMicrosecond() const
	{
		return usec;
	}
	
	int DateTime::getSecond() const
	{
		return sec;
	}
	
	int DateTime::getMinute() const
	{
		return min;
	}
	
	int DateTime::getHour() const
	{
		return hour;
	}
	
	int DateTime::getDayOfMonth() const
	{
		return mday;
	}
	
	int DateTime::getDayOfWeek() const
	{
		return wday;
	}
	
	int DateTime::getDayOfYear() const
	{
		return yday;
	}
	
	int DateTime::getMonth() const
	{
		return mon;
	}
	
	int DateTime::getYear() const
	{
		return year;
	}
	
	int DateTime::getUTCOffset() const
	{
		return utc_offset;
	}
	
	String DateTime::toString() const
	{
		return toString("%Y-%m-%d %H:%M:%S");
	}
	
	String DateTime::toString(const String& format) const
	{
		struct tm tmTime;
		tmTime.tm_sec = sec;
		tmTime.tm_min = min;
		tmTime.tm_hour = hour;
		tmTime.tm_mday = mday;
		tmTime.tm_mon = mon-1;
		tmTime.tm_year = year-DATETIME_TM_BASEYEAR;
		tmTime.tm_wday = wday-1;
		tmTime.tm_yday = yday-1;
		tmTime.tm_isdst = -1;
		#ifdef HAVE_TM_GMTOFF
			tmTime.tm_gmtoff = utc_offset;
		#endif
		String str;
		size_t bytes = 0;
		size_t extendCount = 0;
		while(bytes==0)
		{
			extendCount += 255;
			char* buffer = new char[format.length()+extendCount];
			size_t bytes = strftime(buffer, format.length()+255, format, &tmTime);
			if(bytes>0)
			{
				buffer[bytes] = '\0';
				str = String(buffer, bytes);
			}
			delete[] buffer;
		}
		return str;
	}
	
	String DateTime::toISO8601String() const
	{
		if(utc_offset==0)
		{
			return toString("%Y-%m-%dT%H:%M:%SZ");
		}
		else
		{
			int gmtoff = Math::abs(utc_offset);

			int gmtoff_min = gmtoff%60;
			String gmtoff_min_str;
			gmtoff_min_str += gmtoff_min;
			while(gmtoff_min_str.length() < 2)
			{
				gmtoff_min_str = '0' + gmtoff_min_str;
			}

			int gmtoff_hour = gmtoff/60;
			String gmtoff_hour_str;
			gmtoff_hour_str += gmtoff_hour;
			while(gmtoff_hour_str.length() < 2)
			{
				gmtoff_hour_str = '0' + gmtoff_hour_str;
			}

			time_t timeval = toTimeType();
			struct tm tmTime = *gmtime(&timeval);
			char buffer[26];
			strftime(buffer, 26, "%Y-%m-%dT%H:%M:%S", &tmTime);
			String str = buffer;
			if(utc_offset < 0)
			{
				str += '-';
			}
			else
			{
				str += '+';
			}
			str += gmtoff_hour;
			str += ':';
			str += gmtoff_min;
			return str;
		}
	}
	
	time_t DateTime::toTimeType() const
	{
		struct tm tmTime;
		tmTime.tm_sec = sec;
		tmTime.tm_min = min;
		tmTime.tm_hour = hour;
		tmTime.tm_mday = mday;
		tmTime.tm_mon = mon-1;
		tmTime.tm_year = year-DATETIME_TM_BASEYEAR;
		tmTime.tm_wday = wday-1;
		tmTime.tm_yday = yday-1;
		tmTime.tm_isdst = -1;
		#ifdef _WIN32
		time_t timeval = _mkgmtime(&tmTime);
		#else
		time_t timeval = timegm(&tmTime);
		#endif
		return timeval - (size_t)(60*utc_offset);
	}
}