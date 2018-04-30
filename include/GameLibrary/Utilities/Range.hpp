
#pragma once

namespace fgl
{
	template<typename T>
	class Range
	{
	public:
		T location;
		T length;
		
		Range() : location(0), length(0)
		{
			//
		}
		
		Range(const T& location, const T& length) : location(location), length(length)
		{
			//
		}
		
		T endLocation() const
		{
			return location+length;
		}
	};
}
