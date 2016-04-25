
#pragma once

namespace GameLibrary
{
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
		static float atan2(float x, float y);
		static double atan2(double x, double y);
		static long double atan2(long double x, long double y);

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

		//Random
		static double random();
	};
}








