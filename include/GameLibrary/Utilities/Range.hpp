
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>

namespace fgl
{
	template<typename T>
	class Range
	{
	public:
		T location;
		T length;
		
		constexpr Range() : location(0), length(0) {
			//
		}
		
		constexpr Range(const T& location, const T& length) : location(location), length(length) {
			//
		}
		
		constexpr bool equals(const Range<T>& range) const {
			if(location == range.location && length == range.length) {
				return true;
			}
			return false;
		}
		
		constexpr bool operator==(const Range<T>& range) const {
			return equals(range);
		}
		
		constexpr bool operator!=(const Range<T>& range) const {
			return !equals(range);
		}
		
		constexpr T endLocation() const {
			return location + length;
		}
		
		constexpr T getCenter() const {
			return location + (length/2);
		}
		
		constexpr bool overlaps(const Range<T>& cmp) const {
			if(endLocation() <= cmp.location || cmp.endLocation() <= location) {
				return false;
			}
			return true;
		}
		
		constexpr bool contains(const Range<T>& cmp) const {
			if(cmp.location >= location && cmp.endLocation() <= endLocation()) {
				return true;
			}
			return false;
		}
		
		constexpr void combine(const Range<T>& range) {
			auto end = endLocation();
			auto cmpEnd = range.endLocation();
			if(range.location < location) {
				location = range.location;
				length = end - location;
			}
			if(cmpEnd > end) {
				length = cmpEnd - location;
			}
		}
		
		constexpr Range<T> combined(const Range<T>& range) const {
			auto newRange = *this;
			newRange.combine(range);
			return newRange;
		}
		
		constexpr void translate(double offset) {
			location += offset;
		}
		
		constexpr Range<T> translated(double offset) {
			return Range<T>(location+offset, length);
		}
		
		static constexpr Range<T> fromEdges(const T& start, const T& end) {
			return Range<T>(start, end-start);
		}
		
		String toString() const {
			return "Range(location: " + fgl::stringify<T>(location) + ", length: " + fgl::stringify<T>(length) + ")";
		}
	};
	
	typedef Range<int> RangeI;
	typedef Range<float> RangeF;
	typedef Range<double> RangeD;
	typedef Range<unsigned int> RangeU;
}
