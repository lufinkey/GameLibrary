
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include "Vector2.hpp"

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
		constexpr Line()
		{
			//
		}

		/*! Constructs a line with two points.
			\param x1 the x coordinate for the first point
			\param y1 the y coordinate for the first point
			\param x2 the x coordinate for the second point
			\param y2 the y coordinate for the second point*/
		constexpr Line(const T& x1, const T& y1, const T& x2, const T& y2) : point1(x1,y1), point2(x2, y2)
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
		
		/*! Calculates the center of the line.
			\returns the point in the center of the line endpoints*/
		Vector2<T> getCenter() const
		{
			return Vector2<T>((point1.x+point2.x)/2, (point1.y+point2.y)/2);
		}

		/*! Calculates the slope of the line.
			\returns the value of the slope of the line*/
		template<typename U = T>
		U getSlope() const
		{
			return ((U)point2.y-(U)point1.y)/((U)point2.x - (U)point1.x);
		}

		/*! Tells if the two line segments intersect
			\param line the line segment to check intersection against
			\param intersection outputs the point of intersection between the two line segments, if any
			\returns true if the line segments intersect, false if they do not*/
		bool segmentsIntersect(const Line<T>& line, Vector2<T>* intersection = nullptr) const
		{
			//line intersection code by Kristian Lindberg Vinther
			//https://www.codeproject.com/Tips/862988/Find-the-Intersection-Point-of-Two-Line-Segments

			auto r = point2 - point1;
			auto s = line.point2 - line.point1;
			auto rxs = r.cross(s);
			auto qpxr = (line.point1 - point1).cross(r);

			// If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
			if(rxs==0 && qpxr==0)
			{
				// 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
				// then the two lines are overlapping,
				/*if(considerOverlapAsIntersect)
					if((0 <= (q - p) * r && (q - p) * r <= r * r) || (0 <= (p - q) * s && (p - q) * s <= s * s))
						return true;*/

				// 2. If neither 0 <= (q - p) * r <= r * r nor 0 <= (p - q) * s <= s * s
				// then the two lines are collinear but disjoint.
				// No need to implement this expression, as it follows from the expression above.
				return false;
			}

			// 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
			if(rxs==0 && qpxr!=0)
			{
				return false;
			}

			// t = (q - p) x s / (r x s)
			auto t = (line.point1 - point1).cross(s) / rxs;

			// u = (q - p) x r / (r x s)

			auto u = (line.point1 - point1).cross(r) / rxs;

			// 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
			// the two line segments meet at the point p + t r = q + u s.
			if(rxs!=0 && (0 <= t && t <= 1) && (0 <= u && u <= 1))
			{
				// We can calculate the intersection point using either t or u.
				if(intersection!=nullptr)
				{
					*intersection = point1 + (r * t);
				}

				// An intersection was found.
				return true;
			}

			// 5. Otherwise, the two line segments are not parallel but do not intersect.
			return false;
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
