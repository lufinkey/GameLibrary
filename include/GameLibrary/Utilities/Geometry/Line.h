
#pragma once

#include <GameLibrary/Types.h>
#include <GameLibrary/Utilities/Stringifier.h>
#include "Vector2.h"

namespace fgl
{
	template<typename T>
	/*! Contains data about a line*/
	class Line
	{
	public:
		/*! The coordinates for the first point in the line*/
		fgl::Vector2d point1;
		/*! The coordinates for the second point in the line*/
		fgl::Vector2d point2;

		/*! default constructor*/
		Line()
		{
			//
		}

		/*! Constructs a line with two points.
			\param x1 the x coordinate for the first point
			\param y1 the y coordinate for the first point
			\param x2 the x coordinate for the second point
			\param y2 the y coordinate for the second point*/
		Line(const T& x1, const T& y1, const T& x2, const T& y2) : point1(x1,y1), point2(x2, y2)
		{
			//
		}

		/*! Constructs a line with two points.
			\param point1 the first point
			\param point2 the second point*/
		Line(const Vector2<T>& point1, const Vector2<T>& point2) : point1(point1), point2(point2)
		{
			//
		}

		/*! Calculates the slope of the line.
			\returns the value of the slope of the line*/
		template<typename U = T>
		U getSlope() const
		{
			return ((U)point2.y-(U)point1.y)/((U)point2.x - (U)point1.x);
		}

		/*! Tells if the two line segments intersect
			\param segment the line segment to check intersection against
			\returns true if the line segments intersect, false if they do not*/
		bool segmentsIntersect(const Line<T>& line, Vector2<T>* intersection = nullptr) const
		{
			T s10_x = point2.x - point1.x;
			T s10_y = point2.y - point1.y;
			T s02_x = point1.x - line.point1.x;
			T s02_y = point1.y - line.point1.y;

			T s_numer = (s10_x * s02_y) - (s10_y * s02_x);
			if(s_numer < 0)
			{
				return false; //no intersection
			}

			T s32_x = line.point2.x - line.point1.x;
			T s32_y = line.point2.y - line.point1.y;
			T t_numer = (s32_x * s02_y) - (s32_y * s02_x);
			if(t_numer < 0)
			{
				return false; //no intersection
			}

			T denom = (s10_x * s32_y) - (s32_x * s10_y);
			if(s_numer > denom || t_numer > denom)
			{
				return false; //no intersection
			}

			//intersection
			if(intersection!=nullptr)
			{
				if(std::is_integral<T>())
				{
					long double t = (long double)t_numer / (long double)denom;
					intersection->x = point1.x + (T)(t * (long double)s10_x);
					intersection->y = point1.y + (T)(t * (long double)s10_y);
				}
				else
				{
					T t = t_numer/denom;
					intersection->x = point1.x + (T)(t * s10_x);
					intersection->y = point1.y + (T)(t * s10_y);
				}
			}
			return true;
		}

		String toString() const
		{
			return "Line(("+fgl::stringify<T>(point1.x)+", "+fgl::stringify<T>(point1.y)+"), ("+fgl::stringify<T>(point2.x)+", "+fgl::stringify<T>(point2.y)+"))";
		}
	};

	typedef Line<int> LineI;
	typedef Line<unsigned int> LineU;
	typedef Line<float> LineF;
	typedef Line<double> LineD;
}
