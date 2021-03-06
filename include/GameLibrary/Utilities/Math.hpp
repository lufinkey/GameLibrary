
#pragma once

#include <type_traits>
#include <cmath>

namespace fgl
{
	//TODO turn some of these functions into template functions
	
	class Math
	{
	public:
		static const long double PI;
		
		Math() = delete;

		//Trig
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T sin(T radians) {
			return std::sin(radians);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T cos(T radians) {
			return std::cos(radians);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T tan(T radians) {
			return std::tan(radians);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T asin(T arg) {
			return std::asin(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T acos(T arg) {
			return std::acos(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T atan(T arg) {
			return std::atan(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T atan2(T y, T x) {
			return std::atan2(y, x);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T distance(T x1, T y1, T x2, T y2) {
			T a = x2-x1;
			T b = y2-y1;
			return std::sqrt((a*a)+(b*b));
		}
		
		
		
		//Logarithmic
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T log(T arg) { //natural log
			return std::log(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T log(T base, T arg) {
			return std::log(arg) / std::log(base);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline  T log10(T arg) {
			return std::log10(arg);
		}
		
		
		
		//Power
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T pow(T base, T exponent) {
			return std::pow(base, exponent);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T sqrt(T arg) {
			return std::sqrt(arg);
		}
		
		
		
		//Rounding and remainders
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T ceil(T arg) {
			return std::ceil(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T floor(T arg) {
			return std::floor(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T round(T arg) {
			return std::round(arg);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static T round(T arg, unsigned short precision) {
			if(precision==0) {
				return round(arg);
			}
			T mult = pow(T(10), (T)precision);
			return round(arg*mult)/mult;
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static T roundToMultiple(T arg, T multiple) {
			T numTimes = arg/multiple;
			bool neg = (numTimes < T(0));
			numTimes = floor(abs(numTimes));
			if(fmod(arg, multiple) > (multiple / T(2)))
			{
				numTimes += T(1);
			}
			if(neg)
			{
				return -abs(multiple*numTimes);
			}
			else
			{
				return abs(multiple*numTimes);
			}
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T fmod(T left, T right) {
			return std::fmod(left, right);
		}
		
		
		
		//Absolute Value
		template<typename T, typename std::enable_if<std::is_signed<T>::value, std::nullptr_t>::type = nullptr>
		static inline T abs(T arg) {
			return std::abs(arg);
		}
		
		
		
		//Min and Max
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		static inline T min(T a, T b) {
			return (a<b) ? a : b;
		}
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		static inline T max(T a, T b) {
			return (a>b) ? a : b;
		}
		
		
		
		//Angle Conversions
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T degtorad(T deg) {
			return ((T)PI) * deg / T(180);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T radtodeg(T rad) {
			return rad * T(180) / ((T)PI);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
		static inline T normalizeDegrees(T degrees) {
			if(degrees >= T(0)) {
				return fmod(degrees, T(360));
			}
			else {
				degrees = fmod(degrees, T(360));
				if(degrees < T(0)) {
					return degrees + T(360);
				}
				return degrees;
			}
		}
		
		
		
		//Misc
		static double random();
		
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value && std::is_signed<T>::value, std::nullptr_t>::type = nullptr>
		static constexpr T sign(T value) {
			if(value > 0) {
				return T(1);
			}
			else if(value < 0) {
				return T(-1);
			}
			return T(0);
		}
	};
}








