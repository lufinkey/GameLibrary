
#pragma once

#include <GameLibrary/Utilities/Stringifier.hpp>

namespace fgl
{
	template<typename T>
	/*! Contains data about a circle*/
	class Circle
	{
	public:
		/*! The x coordinate for the center of the circle*/
		T x;
		/*! The y coordinate for the center of the circle*/
		T y;
		/*! The radius of the circle*/
		T radius;
		
		/*! default constructor*/
		constexpr Circle() : x(0), y(0), radius(0)
		{
			//
		}
		
		/*! Constructs a circle with a center point and a radius.
			\param x the x coordinate for the center point
			\param y the y coordinate for the center point
			\param radius the radius*/
		constexpr Circle(const T& x, const T& y, const T& radius) : x(x), y(y), radius(radius)
		{
			//
		}
		
		String toString() const
		{
			return "Circle(x:" + fgl::stringify<T>(x) + ", y:" + fgl::stringify<T>(y) + ", radius:" + fgl::stringify<T>(radius) + ")";
		}
	};
	
	typedef Circle<int> CircleI;
	typedef Circle<unsigned int> CircleU;
	typedef Circle<float> CircleF;
	typedef Circle<double> CircleD;
}
