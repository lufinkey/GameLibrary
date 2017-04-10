
#include <GameLibrary/Utilities/Math.hpp>
#include <cmath>
#include <cstdlib>

namespace fgl
{
	const long double Math::PI = 3.14159265358979323846264338328L;
	
	//Trig
	float Math::sin(float radians) { return std::sin(radians); }
	double Math::sin(double radians){ return std::sin(radians); }
	long double Math::sin(long double radians){ return std::sin(radians); }
	float Math::cos(float radians) { return std::cos(radians); }
	double Math::cos(double radians){ return std::cos(radians); }
	long double Math::cos(long double radians){ return std::cos(radians); }
	float Math::tan(float radians) { return std::tan(radians); }
	double Math::tan(double radians){ return std::tan(radians); }
	long double Math::tan(long double radians){ return std::tan(radians); }
	float Math::asin(float arg) { return std::asin(arg); }
	double Math::asin(double arg){ return std::asin(arg); }
	long double Math::asin(long double arg){ return std::asin(arg); }
	float Math::acos(float arg) { return std::acos(arg); }
	double Math::acos(double arg){ return std::acos(arg); }
	long double Math::acos(long double arg){ return std::acos(arg); }
	float Math::atan(float arg) { return std::atan(arg); }
	double Math::atan(double arg){ return std::atan(arg); }
	long double Math::atan(long double x){ return std::atan(x); }
	float Math::atan2(float x, float y) { return std::atan2(x, y); }
	double Math::atan2(double x, double y){ return std::atan2(x, y); }
	long double Math::atan2(long double x, long double y){ return std::atan2(x, y); }
	float Math::distance(float x1, float y1, float x2, float y2)
	{
		float a = x2-x1;
		float b = y2-y1;
		return std::sqrt((a*a)+(b*b));
	}
	double Math::distance(double x1, double y1, double x2, double y2)
	{
		double a = x2-x1;
		double b = y2-y1;
		return std::sqrt((a*a)+(b*b));
	}
	long double Math::distance(long double x1, long double y1, long double x2, long double y2)
	{
		long double a = x2-x1;
		long double b = y2-y1;
		return std::sqrt((a*a)+(b*b));
	}
	
	//Logarithmic
	float Math::log(float arg) { return std::log(arg); }
	double Math::log(double arg){ return std::log(arg); }
	long double Math::log(long double arg){ return std::log(arg); }
	float Math::log(float base, float arg) { return std::log(arg) / std::log(base); }
	double Math::log(double base, double arg){ return std::log(arg) / std::log(base); }
	long double Math::log(long double base, long double arg){ return std::log(arg) / std::log(base); }
	float Math::log10(float arg) { return std::log10(arg); }
	double Math::log10(double arg){ return std::log10(arg); }
	long double Math::log10(long double arg){ return std::log10(arg); }
	
	//Powers
	float Math::pow(float base, float exponent) { return std::pow(base, exponent); }
	double Math::pow(double base, double exponent){ return std::pow(base, exponent); }
	long double Math::pow(long double base, long double exponent){ return std::pow(base, exponent); }
	float Math::sqrt(float arg) { return std::sqrt(arg); }
	double Math::sqrt(double arg){ return std::sqrt(arg); }
	long double Math::sqrt(long double arg){ return std::sqrt(arg); }
	
	//Rounding and remainders
	float Math::ceil(float arg) { return std::ceil(arg); }
	double Math::ceil(double arg){ return std::ceil(arg); }
	long double Math::ceil(long double arg){ return std::ceil(arg); }
	float Math::floor(float arg) { return std::floor(arg); }
	double Math::floor(double arg){ return std::floor(arg); }
	long double Math::floor(long double arg){ return std::floor(arg); }
	float Math::round(float arg) { return std::round(arg); }
	double Math::round(double arg){ return std::round(arg); }

	float Math::round(float arg, unsigned int precision)
	{
		if(precision==0)
		{
			return round(arg);
		}
		float mult = pow(10, (float)precision);
		return round(arg*mult)/mult;
	}
	double Math::round(double arg, unsigned int precision)
	{
		if(precision==0)
		{
			return round(arg);
		}
		double mult = pow(10, (double)precision);
		return round(arg*mult)/mult;
	}
	long double Math::round(long double arg, unsigned int precision)
	{
		if(precision==0)
		{
			return round(arg);
		}
		long double mult = pow(10, (long double)precision);
		return round(arg*mult)/mult;
	}

	float Math::roundToMultiple(float arg, float multiple)
	{
		float numTimes = arg/multiple;
		bool neg = (numTimes < 0);
		numTimes = floor(abs(numTimes));
		if(fmod(arg, multiple) > (multiple / 2))
		{
			numTimes+=1;
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

	double Math::roundToMultiple(double arg, double multiple)
	{
		double numTimes = arg/multiple;
		bool neg = (numTimes < 0);
		numTimes = floor(abs(numTimes));
		if(fmod(arg, multiple) >(multiple / 2))
		{
			numTimes+=1;
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

	long double Math::roundToMultiple(long double arg, long double multiple)
	{
		long double numTimes = arg/multiple;
		bool neg = (numTimes < 0);
		numTimes = floor(abs(numTimes));
		if(fmod(arg, multiple) >(multiple / 2))
		{
			numTimes+=1;
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

	long double Math::round(long double arg){ return std::round(arg); }
	float Math::fmod(float left, float right){ return std::fmod(left, right); }
	double Math::fmod(double left, double right){ return std::fmod(left, right); }
	long double Math::fmod(long double left, long double right){ return std::fmod(left, right); }
	
	//Absolute Value
	float Math::abs(float arg) { return std::abs(arg); }
	double Math::abs(double arg){ return std::abs(arg); }
	long double Math::abs(long double arg){ return std::abs(arg); }
	int Math::abs(int arg){ return std::abs(arg); }
	long Math::abs(long arg){ return std::abs(arg); }
	long long Math::abs(long long arg) { return std::abs(arg); }
	
	//Min and Max
	float Math::min(float a, float b) { return (a<b) ? a : b; }
	double Math::min(double a, double b) { return (a<b) ? a : b; }
	long double Math::min(long double a, long double b) { return (a<b) ? a : b; }
	int Math::min(int a, int b) { return (a<b) ? a : b; }
	long Math::min(long a, long b) { return (a<b) ? a : b; }
	long long Math::min(long long a, long long b) { return (a<b) ? a : b; }
	float Math::max(float a, float b) { return (a>b) ? a : b; }
	double Math::max(double a, double b) { return (a>b) ? a : b; }
	long double Math::max(long double a, long double b) { return (a>b) ? a : b; }
	int Math::max(int a, int b) { return (a>b) ? a : b; }
	long Math::max(long a, long b) { return (a>b) ? a : b; }
	long long Math::max(long long a, long long b) { return (a>b) ? a : b; }

	//Angle Conversions
	float Math::degtorad(float deg) { return (float)(PI * deg / 180); }
	double Math::degtorad(double deg) { return (double)(PI * deg / 180); }
	long double Math::degtorad(long double deg) { return (long double)(PI * deg / 180); }
	float Math::radtodeg(float rad) { return (float)(rad * 180.0f / (float)PI); };
	double Math::radtodeg(double rad){ return (double)(rad * 180.0 / (double)PI); };
	long double Math::radtodeg(long double rad) { return (long double)(rad * 180.0L / (long double)PI); };
	
	float Math::normalizeDegrees(float degrees)
	{
		if(degrees >= 0)
		{
			return fmod(degrees, 360);
		}
		else
		{
			degrees = fmod(degrees, 360);
			if(degrees < 0)
			{
				return degrees + 360;
			}
			return degrees;
		}
	}
	
	double Math::normalizeDegrees(double degrees)
	{
		if(degrees >= 0)
		{
			return fmod(degrees, 360);
		}
		else
		{
			degrees = fmod(degrees, 360);
			if(degrees < 0)
			{
				return degrees + 360;
			}
			return degrees;
		}
	}
	
	long double normalizeDegrees(long double degrees)
	{
		if(degrees >= 0)
		{
			return fmod(degrees, 360);
		}
		else
		{
			degrees = fmod(degrees, 360);
			if(degrees < 0)
			{
				return degrees + 360;
			}
			return degrees;
		}
	}

	//Random
	double Math::random() { return ((double)std::rand())/((double)(RAND_MAX)); }
}

