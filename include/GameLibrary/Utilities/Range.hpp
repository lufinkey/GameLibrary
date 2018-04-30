
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
	
	typedef Range<int> RangeI;
	typedef Range<float> RangeF;
	typedef Range<double> RangeD;
	typedef Range<unsigned int> RangeU;
}
