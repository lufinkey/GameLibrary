
#pragma once

namespace fgl
{
	//TODO turn some of these functions into template functions
	
	class Math
	{
	public:
		static const long double PI;

		//Trig
		static float sin(float radians);
		static double sin(double radians);
		static long double sin(long double radians);
		static float cos(float radians);
		static double cos(double radians);
		static long double cos(long double radians);
		static float tan(float radians);
		static double tan(double radians);
		static long double tan(long double radians);
		static float asin(float arg);
		static double asin(double arg);
		static long double asin(long double arg);
		static float acos(float arg);
		static double acos(double arg);
		static long double acos(long double arg);
		static float atan(float arg);
		static double atan(double arg);
		static long double atan(long double arg);
		static float atan2(float y, float x);
		static double atan2(double y, double x);
		static long double atan2(long double y, long double x);
		static float distance(float x1, float y1, float x2, float y2);
		static double distance(double x1, double y1, double x2, double y2);
		static long double distance(long double x1, long double y1, long double x2, long double y2);

		//Logarithmic
		static float log(float arg); //natural log
		static double log(double arg); //natural log
		static long double log(long double arg); //natural log
		static float log(float base, float arg);
		static double log(double base, double arg);
		static long double log(long double base, long double arg);
		static float log10(float arg);
		static double log10(double arg);
		static long double log10(long double arg);

		//Power
		static float pow(float base, float exponent);
		static double pow(double base, double exponent);
		static long double pow(long double base, long double exponent);
		static float sqrt(float arg);
		static double sqrt(double arg);
		static long double sqrt(long double arg);

		//Rounding and remainders
		static float ceil(float arg);
		static double ceil(double arg);
		static long double ceil(long double arg);
		static float floor(float arg);
		static double floor(double arg);
		static long double floor(long double arg);
		static float round(float arg);
		static double round(double arg);
		static long double round(long double arg);
		static float round(float arg, unsigned int precision);
		static double round(double arg, unsigned int precision);
		static long double round(long double arg, unsigned int precision);
		static float roundToMultiple(float arg, float multiple);
		static double roundToMultiple(double arg, double multiple);
		static long double roundToMultiple(long double arg, long double multiple);
		static float fmod(float left, float right);
		static double fmod(double left, double right);
		static long double fmod(long double left, long double right);

		//Absolute Value
		static float abs(float arg);
		static double abs(double arg);
		static long double abs(long double arg);
		static int abs(int arg);
		static long abs(long arg);
		static long long abs(long long arg);
		
		//Min and Max
		static float min(float a, float b);
		static double min(double a, double b);
		static long double min(long double a, long double b);
		static int min(int a, int b);
		static long min(long a, long b);
		static long long min(long long a, long long b);
		static float max(float a, float b);
		static double max(double a, double b);
		static long double max(long double a, long double b);
		static int max(int a, int b);
		static long max(long a, long b);
		static long long max(long long a, long long b);

		//Angle Conversions
		static float degtorad(float deg);
		static double degtorad(double deg);
		static long double degtorad(long double deg);
		static float radtodeg(float rad);
		static double radtodeg(double rad);
		static long double radtodeg(long double rad);
		static float normalizeDegrees(float degrees);
		static double normalizeDegrees(double degrees);
		static long double normalizeDegrees(long double degrees);

		//Random
		static double random();
	};
}








