
#pragma once

#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class TimeInterval
	{
	public:
		TimeInterval();
		TimeInterval(const TimeInterval&);
		explicit TimeInterval(long long milliseconds);
		~TimeInterval();
		
		TimeInterval& operator=(const TimeInterval&);
		
		bool equals(const TimeInterval&) const;
		bool operator==(const TimeInterval&) const;
		bool operator!=(const TimeInterval&) const;
		
		bool operator<(const TimeInterval&) const;
		bool operator<=(const TimeInterval&) const;
		bool operator>(const TimeInterval&) const;
		bool operator>=(const TimeInterval&) const;
		
		TimeInterval operator+(const TimeInterval&) const;
		TimeInterval operator-(const TimeInterval&) const;
		
		TimeInterval& operator+=(const TimeInterval&);
		TimeInterval& operator-=(const TimeInterval&);
		
		void start();
		void stop();
		void reset();
		
		long long getMilliseconds() const;
		
		String toString() const;
		
	private:
		long long milliseconds;
		long long lastmillis;
		bool running;
		
		void update();
	};
}

REGISTER_TYPE(fgl::TimeInterval)
