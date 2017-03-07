
#pragma once

#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include "Vector2.hpp"

namespace fgl
{
	template<typename T>
	/*! A series of points that make up a polygon.*/
	class Polygon
	{
	public:
		/*! default constructor*/
		Polygon()
		{
			//
		}
		
		/*! Constructs a Polygon from an array of x coordinates and an array of y coordinates.
			\param xpoints an array of x coordinates
			\param ypoints an array of y coordinates
			\param numpoints the size of the xpoints and ypoints arrays*/
		Polygon(const T* xpoints, const T* ypoints, unsigned int numpoints)
		{
			for (unsigned int i=0; i<numpoints; i++)
			{
				points.add(Vector2<T>(xpoints[i], ypoints[i]));
			}
		}
		
		/*! Constructs a Polygon from an ArrayList of coordinates.
			\param points a list of points in the polygon*/
		explicit Polygon(const ArrayList<Vector2<T>>& points)
		{
			Polygon<T>::points = points;
		}
		
		/*! Constructs a Polygon from an array of coordinates.
			\param points an array of points in the polygon
			\param numpoints the size of the points array*/
		Polygon(const Vector2<T>* points, unsigned int numpoints)
		{
			for(unsigned int i=0; i<numpoints; i++)
			{
				Polygon::points.add(points[i]);
			}
		}
		
		/*! Adds a point to the set of points in the polygon.
			\param x the x coordinate of the point
			\param y the y coordinate of the point*/
		void addPoint(const T& x, const T& y)
		{
			points.add(Vector2<T>(x,y));
		}
		
		/*! Adds a point to the set of points in the polygon.
			\param point the coordinates of the point*/
		void addPoint(const Vector2<T>& point)
		{
			points.add(point);
		}
		
		/*! Gets a list of all of the points in the polygon.
			\returns a const ArrayList reference of Vector2d objects*/
		const ArrayList<Vector2<T>>& getPoints() const
		{
			return points;
		}
		
		/*! Tells whether a given point is within the polygon.
			\param x the x coordinate of the point to check
			\param y the y coordinate of the point to check
			\returns true if the point is inside of the polygon, or false if otherwise*/
		bool contains(const T& x, const T& y) const
		{
			if(points.size() == 0)
			{
				return false;
			}
			size_t length = points.size();
			size_t i = 0;
			size_t j = 0;
			bool c = false;
			for(i=0, j =(length-1); i<length; j=i,i++)
			{
				if (((points[i].y > y) != (points[j].y > y)) &&
					(x < (points[j].x - points[i].x) * (y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
				{
					c = !c;
				}
			}
			return c;
		}
		
		/*! Tells whether a given point is within the polygon.
			\param point the coordinates of the point to check
			\returns true if the point is inside of the polygon, or false if otherwise*/
		bool contains(const Vector2<T>& point) const
		{
			return contains(point.x, point.y);
		}
		
		/*! Removes all points.*/
		void clear()
		{
			points.clear();
		}
		
		String toString() const
		{
			size_t size = points.size();
			size_t lastIndex = size-1;
			String str = "Polygon( ";
			for(size_t i=0; i<points.size(); i++)
			{
				const Vector2<T>& point = points.get(i);
				str += "(" + fgl::stringify<T>(point.x) + ", " + fgl::stringify<T>(point.y) + ")";
				if(i != lastIndex)
				{
					str += ", ";
				}
				else
				{
					str += ' ';
				}
			}
			str += ")";
			return str;
		}
		
	private:
		ArrayList<Vector2<T>> points;
	};
	
	typedef Polygon<int> PolygonI;
	typedef Polygon<float> PolygonF;
	typedef Polygon<double> PolygonD;
	typedef Polygon<long double> PolygonLD;
}
