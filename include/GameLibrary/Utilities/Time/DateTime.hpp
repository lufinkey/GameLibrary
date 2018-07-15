
#pragma once

#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class DateTime
	{
	public:
		/* returns the local UTC offset in minutes (-720 - 720) */
		static int getLocalUTCOffset();
		
		/* Constructs a DateTime object with the current date and time */
		DateTime();
		/* Constructs local DateTime from GMT time_t */
		DateTime(const time_t&);
		
		/* DateTime destructor */
		~DateTime();
		
		void assign(const DateTime&);
		void assign(const time_t&);
		
		int getMicrosecond() const;
		int getSecond() const;
		int getMinute() const;
		int getHour() const;
		int getDayOfMonth() const;
		int getDayOfWeek() const;
		int getDayOfYear() const;
		int getMonth() const;
		int getYear() const;
		/* returns the DateTime's offset from UTC time in minutes (-720 - 720) */
		int getUTCOffset() const;
		
		String toString() const; //default format: %Y-%m-%d %H:%M:%S %z
		String toString(const String& format) const;
		String toISO8601String() const;
		time_t toTimeType() const;
		
	private:
		/* UTC offset in minutes (-720 - 720) */
		int utc_offset;

		/* microsecond (000000-999999) */
		int usec;

		/* second (00-61) (generally 00-59. the extra 2 seconds are for leap seconds) */
		int sec;

		/* minute (00-59) */
		int min;

		/* hour (00-23) */
		int hour;

		/* day of the month (01-31) */
		int mday;

		/* month (01-12) */
		int mon;

		/* year (0000-9999) */
		int year;

		/* day of the week (sunday=1) (1-7) */
		int wday;

		/* day of the year (001-366) */
		int yday;
	};
}

REGISTER_TYPE(fgl, DateTime)
