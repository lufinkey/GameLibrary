
#pragma once

#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/Math.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include "Vector2.hpp"
#include "Line.hpp"

namespace fgl
{
	template<typename T>
	class Rectangle;
	
	template<typename T>
	/*! A series of points that make up a polygon.*/
	class Polygon
	{
	public:
		typedef typename ArrayList<Vector2<T>>::iterator iterator;
		typedef typename ArrayList<Vector2<T>>::const_iterator const_iterator;
		typedef typename ArrayList<Vector2<T>>::reverse_iterator reverse_iterator;
		typedef typename ArrayList<Vector2<T>>::const_reverse_iterator const_reverse_iterator;
	
		/*! default constructor*/
		Polygon()
		{
			//
		}
		
		/*! Constructs a Polygon from an array of x coordinates and an array of y coordinates.
			\param xpoints an array of x coordinates
			\param ypoints an array of y coordinates
			\param numpoints the size of the xpoints and ypoints arrays*/
		Polygon(const T* xpoints, const T* ypoints, size_t numpoints)
		{
			points.reserve(numpoints);
			for(size_t i=0; i<numpoints; i++)
			{
				points.add(Vector2<T>(xpoints[i], ypoints[i]));
			}
		}
		
		/*! Constructs a Polygon from an ArrayList of coordinates.
			\param points a list of points in the polygon*/
		explicit Polygon(const ArrayList<Vector2<T>>& points)
			: points(points)
		{
			//
		}

		/*! Constructs a Polygon from an initializer_list of Vector2 objects
			\param points an initializer_list of points in the polygon */
		Polygon(std::initializer_list<Vector2<T>> points)
			: points(points)
		{
			//
		}
		
		/*! Constructs a Polygon from an array of coordinates.
			\param points an array of points in the polygon
			\param numpoints the size of the points array*/
		Polygon(const Vector2<T>* points, size_t numpoints)
		{
			for(unsigned int i=0; i<numpoints; i++)
			{
				Polygon::points.add(points[i]);
			}
		}
		
		iterator begin()
		{
			return points.begin();
		}
		
		const_iterator begin() const
		{
			return points.begin();
		}
		
		const_iterator cbegin() const
		{
			return points.cbegin();
		}
		
		iterator end()
		{
			return points.end();
		}
		
		const_iterator end() const
		{
			return points.end();
		}
		
		const_iterator cend() const
		{
			return points.cend();
		}
		
		reverse_iterator rbegin()
		{
			return points.rbegin();
		}
		
		const_reverse_iterator rbegin() const
		{
			return points.rbegin();
		}
		
		const_reverse_iterator crbegin() const
		{
			return points.crbegin();
		}
		
		reverse_iterator rend()
		{
			return points.rend();
		}
		
		const_reverse_iterator rend() const
		{
			return points.rend();
		}
		
		const_reverse_iterator crend() const
		{
			return points.crend();
		}
		
		Vector2<T>& operator[](size_t index)
		{
			return points[index];
		}
		
		const Vector2<T>& operator[](size_t index) const
		{
			return points[index];
		}
		
		/*! Adds a point to the array of points in the polygon.
			\param x the x coordinate of the point
			\param y the y coordinate of the point*/
		void addPoint(const T& x, const T& y)
		{
			points.add(Vector2<T>(x,y));
		}
		
		/*! Adds a point to the array of points in the polygon.
			\param point the coordinates of the point*/
		void addPoint(const Vector2<T>& point)
		{
			points.add(point);
		}

		/*! Inserts a point to a given index in the array of points in the polygon.
			\param index the index to insert the point at
			\param point the coordinates of the point*/
		void addPoint(size_t index, const Vector2<T>& point)
		{
			points.add(index, point);
		}

		/*! Removes a point from a given index in the array of points in the polygon.
			\param index the index of the point*/
		void removePoint(size_t index)
		{
			points.remove(index);
		}

		const Vector2<T>& getPoint(size_t index) const
		{
			return points[index];
		}

		/*! Gets the number of points in the polygon.
			\returns a size_t value representing the total number of vertices in the polygon */
		size_t getPointCount() const
		{
			return points.size();
		}
		
		/*! Gets an array of all of the points in the polygon.
			\returns a const ArrayList reference of Vector2d objects*/
		const ArrayList<Vector2<T>>& getPoints() const
		{
			return points;
		}

		/*! Gets an array of all of the edges in the polygon
			\returns an ArrayList of Line objects */
		ArrayList<Line<T>> getEdges() const
		{
			if(points.size()<2)
			{
				return {};
			}
			fgl::ArrayList<Line<T>> edges;
			edges.reserve(points.size());
			auto firstPoint = points[0];
			auto lastPoint = firstPoint;
			for(size_t points_size=points.size(), i=1; i<points_size; i++)
			{
				auto point = points[i];
				edges.add(Line<T>(lastPoint, point));
				lastPoint = point;
			}
			if(points.size()>=3)
			{
				edges.add(Line<T>(lastPoint, firstPoint));
			}
			return edges;
		}

		/*! Calculates the centroid of the polygon, or the average of all of the points in the shape
			\returns a Vector2 that represents the average of all of the points in the polygon */
		Vector2<T> getCentroid() const
		{
			if(points.size()==0)
			{
				return Vector2<T>(0, 0);
			}
			Vector2<T> total;
			for(auto& point : points)
			{
				total += point;
			}
			return total / (T)points.size();
		}


		/*! Checks if the edges progress clockwise, meaning the right side of each edge is the interior of the polygon
			\returns true if the edges progress clockwise, or false if the edges progress counter-clockwise */
		bool isClockwise() const
		{
			if(points.size()<=1)
			{
				return true;
			}
			T total = 0;
			auto& last = points[0];
			for(size_t i=1; i<points.size(); i++)
			{
				auto& current = points[i];
				total += (current.x - last.x) * (current.y + last.y);
			}
			if(total < 0)
			{
				return false;
			}
			return true;
		}

		/*! Calculates the area of this polygon
			\returns the area of the polygon */
		T getArea() const
		{
			T area = 0;

			size_t point_count = points.size();
			size_t j = point_count-1;

			for(size_t i=0; i<point_count; i++)
			{
				auto lastPoint = points[j];
				auto point = points[i];
				area += (lastPoint.x + point.x) * (lastPoint.y - point.y);
				j = i;
			}

			return fgl::Math::abs(area / 2);
		}

		/*! Creates a convex polygon using the points from this polygon
			\returns a convex polygon */
		Polygon<T> makeConvex() const
		{
			static const double SIN_OF_MIN_ANGLE = Math::sin(Math::degtorad(5.0));
			if(points.size() <= 3)
			{
				return *this;
			}
			auto points = this->points;
			if(isClockwise())
			{
				points = points.reversed();
			}
			Polygon<T> convex;
			convex.addPoint(points[0]);
			convex.addPoint(points[1]);
			auto last = points[1];
			auto lastIn = points[1]-points[0];
			for(size_t i=2; i<points.size(); i++)
			{
				auto current = points[i];
				auto currentIn = current-last;
				auto cross = lastIn.cross(currentIn);
				bool convexAngle = false;
				if(cross <= -SIN_OF_MIN_ANGLE)
				{
					convexAngle = true;
				}
				if(!convexAngle)
				{
					// last is not part of the convex hull
					while(!convexAngle && convex.points.size() > 3)
					{
						convex.points.remove(convex.points.size()-1);
						size_t cl = convex.points.size();
						currentIn = current-convex.points[cl-1];
						if(cl >= 2)
						{
							lastIn = convex.points[cl-1]-convex.points[cl-2];
							cross = lastIn.cross(currentIn);
							if(cross <= -SIN_OF_MIN_ANGLE)
							{
								convexAngle = true;
							}
						}
						else
						{
							convexAngle = true;
						}

					}
				}
				convex.addPoint(current);
				lastIn = currentIn;
				last = current;
			}
			return convex;
		}

		/*! Simplifies the polygon to the given number of points
			\param numPoints the maximum number of points that the polygon should contain at the end of this process */
		void simplify(size_t numPoints)
		{
			if(numPoints==0)
			{
				points.clear();
				return;
			}
			else if(numPoints==1)
			{
				if(points.size() > 0)
				{
					auto centroid = getCentroid();
					points.clear();
					points.add(centroid);
				}
				return;
			}
			while(points.size() > numPoints)
			{
				if(points.size() > 3)
				{
					//remove the middle point of the triangle with the smallest area
					T lowestArea = 0;
					size_t lowestAreaIndex = -1;
					for(size_t i=0; i<points.size(); i++)
					{
						size_t prevIndex = i-1;
						if(i==0)
						{
							prevIndex = points.size()-1;
						}
						size_t nextIndex = i+1;
						if(i == (points.size()-1))
						{
							nextIndex = 0;
						}
						auto prev = points[prevIndex];
						auto curr = points[i];
						auto next = points[nextIndex];
						Polygon<T> triangle = {prev, curr, next};
						auto area = triangle.getArea();
						if(area < lowestArea || lowestAreaIndex==-1)
						{
							lowestArea = area;
							lowestAreaIndex = i;
						}
					}
					points.remove(lowestAreaIndex);
				}
				else
				{
					//remove the point not included in the longest edge
					T longestEdge = 0;
					size_t longestEdgeIndex = -1;
					for(size_t i=0; i<points.size(); i++)
					{
						size_t prevIndex = i-1;
						if(i==0)
						{
							prevIndex = points.size()-1;
						}
						auto curr = points[i];
						auto prev = points[prevIndex];
						auto A = curr.x - prev.x;
						auto B = curr.y - prev.y;
						auto edge = (A*A) + (B*B);
						if(edge > longestEdge || longestEdgeIndex==-1)
						{
							longestEdge = edge;
							longestEdgeIndex = i;
						}
					}

					ArrayList<size_t> indexes;
					indexes.reserve(points.size());
					for(size_t i=0; i<points.size(); i++)
					{
						indexes.add(i);
					}
					indexes.removeFirstEqual(longestEdgeIndex);
					size_t prevLongestEdgeIndex = longestEdgeIndex-1;
					if(longestEdgeIndex==0)
					{
						prevLongestEdgeIndex = points.size()-1;
					}
					indexes.removeFirstEqual(prevLongestEdgeIndex);

					size_t indexToRemove = indexes[0];
					points.remove(indexToRemove);
				}
			}
		}

		/*! Removes all redundant points in the Polygon, meaning points that don't create an angle */
		void removeRedundancies(T threshold = 0)
		{
			threshold = Math::abs(threshold);
			for(size_t i=(points.size()-1); i!=-1 && points.size() > 3; i--)
			{
				size_t prevIndex = i-1;
				if(i==0)
				{
					prevIndex = points.size()-1;
				}
				size_t nextIndex = i+1;
				if(i == (points.size()-1))
				{
					nextIndex = 0;
				}
				auto prev = points[prevIndex];
				auto curr = points[i];
				auto next = points[nextIndex];
				auto v1 = curr - prev;
				auto v2 = next - curr;
				auto cross = Math::abs(v1.cross(v2));
				auto dot = v1.dot(v2);
				if(cross <= threshold && dot > 0)
				{
					points.remove(i);
				}
			}
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
			for(i=0, j=(length-1); i<length; j=i, i++)
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

		/*! Tells if a given polygon overlaps with this polygon
			\param polygon the polygon to compare points and edges with
			\returns true if the given polygon overlaps this polygon, or false if they do not overlap */
		bool intersects(const Polygon<T>& polygon)
		{
			if(points.size()<=1)
			{
				return false;
			}
			if(polygon.points.size()<=1)
			{
				return false;
			}
			//check if any edges intersect
			auto edges = getEdges();
			auto polyEdges = polygon.getEdges();
			for(auto& line : edges)
			{
				for(auto& polyLine : polyEdges)
				{
					if(line.segmentsIntersect(polyLine))
					{
						return true;
					}
				}
			}
			//check if any point from each polygon lies inside of the other polygon
			if(polygon.contains(points[0]))
			{
				return true;
			}
			if(contains(polygon.points[0]))
			{
				return true;
			}
			return false;
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
		
		Rectangle<T> getRectangle() const;
		
	private:
		ArrayList<Vector2<T>> points;
	};
	
	typedef Polygon<int> PolygonI;
	typedef Polygon<float> PolygonF;
	typedef Polygon<double> PolygonD;
	typedef Polygon<long double> PolygonLD;
}
