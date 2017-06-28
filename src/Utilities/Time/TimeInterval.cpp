
#include <GameLibrary/Utilities/Time/TimeInterval.hpp>
#include <chrono>

namespace fgl
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;
	TimePoint TimeInterval_startTime = Clock::now();

	long long TimeInterval_getCurrentMilliseconds()
	{
		auto now = Clock::now();
		auto millis = now - TimeInterval_startTime;
		return (long long)std::chrono::duration_cast<std::chrono::milliseconds>(millis).count();
	}

	TimeInterval::TimeInterval()
		: milliseconds(0),
		lastmillis(0),
		running(false)
	{
		//
	}

	TimeInterval::TimeInterval(const TimeInterval& time)
		: milliseconds(time.getMilliseconds()),
		lastmillis(time.lastmillis),
		running(false)
	{
		//
	}

	TimeInterval::TimeInterval(long long millis)
		: milliseconds(0),
		lastmillis(0),
		running(false)
	{
		//
	}

	TimeInterval::~TimeInterval()
	{
		//
	}

	TimeInterval& TimeInterval::operator=(const TimeInterval& time)
	{
		lastmillis = TimeInterval_getCurrentMilliseconds();
		milliseconds = time.getMilliseconds();
		return *this;
	}
	
	bool TimeInterval::equals(const TimeInterval&time) const
	{
		if(getMilliseconds() == time.getMilliseconds())
		{
			return true;
		}
		else if(time.getMilliseconds() == getMilliseconds())
		{
			return true;
		}
		return false;
	}
	
	bool TimeInterval::operator==(const TimeInterval&time) const
	{
		return equals(time);
	}
	
	bool TimeInterval::operator!=(const TimeInterval&time) const
	{
		return !equals(time);
	}
	
	bool TimeInterval::operator<(const TimeInterval&time) const
	{
		if(getMilliseconds() < time.getMilliseconds())
		{
			return true;
		}
		return false;
	}
	
	bool TimeInterval::operator<=(const TimeInterval&time) const
	{
		if(getMilliseconds() <= time.getMilliseconds())
		{
			return true;
		}
		return false;
	}
	
	bool TimeInterval::operator>(const TimeInterval&time) const
	{
		if(getMilliseconds() > time.getMilliseconds())
		{
			return true;
		}
		return false;
	}
	
	bool TimeInterval::operator>=(const TimeInterval&time) const
	{
		if(getMilliseconds() >= time.getMilliseconds())
		{
			return true;
		}
		return false;
	}
	
	TimeInterval TimeInterval::operator+(const TimeInterval&time) const
	{
		return TimeInterval(getMilliseconds() + time.getMilliseconds());
	}
	
	TimeInterval TimeInterval::operator-(const TimeInterval&time) const
	{
		return TimeInterval(getMilliseconds() - time.getMilliseconds());
	}
	
	TimeInterval& TimeInterval::operator+=(const TimeInterval&time)
	{
		milliseconds += time.getMilliseconds();
		return *this;
	}
	
	TimeInterval& TimeInterval::operator-=(const TimeInterval&time)
	{
		milliseconds -= time.getMilliseconds();
		return *this;
	}
	
	void TimeInterval::update()
	{
		long long currentmillis = TimeInterval_getCurrentMilliseconds();
		milliseconds += (currentmillis - lastmillis);
		lastmillis = currentmillis;
	}
	
	void TimeInterval::start()
	{
		if(!running)
		{
			lastmillis = TimeInterval_getCurrentMilliseconds();
			running = true;
		}
	}

	void TimeInterval::stop()
	{
		if(running)
		{
			running = false;
			update();
		}
	}

	void TimeInterval::reset()
	{
		milliseconds = 0;
		lastmillis = TimeInterval_getCurrentMilliseconds();
	}

	long long TimeInterval::getMilliseconds() const
	{
		if(running)
		{
			long long currentmillis = TimeInterval_getCurrentMilliseconds();
			return milliseconds + (currentmillis - lastmillis);
		}
		return milliseconds;
	}
	
	String TimeInterval::toString() const
	{
		return (String)"" + milliseconds;
	}
}
