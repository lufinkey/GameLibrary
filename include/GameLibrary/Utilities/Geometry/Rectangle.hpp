
#pragma once

#include <GameLibrary/Utilities/Range.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include "Vector2.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

namespace fgl
{
	template<typename T>
	/*! Contains data about a rectangle*/
	class Rectangle
	{
	public:
		/*! The x coordinate of the top left of the rectangle*/
		T x;
		/*! The y coordinate of the top left of the rectangle*/
		T y;
		/*! The width of the rectangle*/
		T width;
		/*! The height of the rectangle*/
		T height;
		
		/*! default constructor*/
		constexpr Rectangle() : x(0), y(0), width(0), height(0) {
			//
		}
		
		/*! Constructs a rectangle with a given top left coordinate and size.
			\param x the x coordinate of the top left of the rectangle
			\param y the y coordinate of the top left of the rectangle
			\param width the width of the rectangle
			\param height the height of the rectangle*/
		constexpr Rectangle(const T& x, const T& y, const T& width, const T& height) : x(x), y(y), width(width), height(height) {
			//
		}

		/*! Explicit conversion between Rectangle template types. 
			\param rectangle the rectangle being converted from*/
		template<typename U>
		explicit Rectangle(const Rectangle<U>& rectangle) : x((U)rectangle.x), y((U)rectangle.y), width((U)rectangle.width), height((U)rectangle.height) {
			//
		}
		
		constexpr bool equals(const Rectangle<T>& rectangle) const {
			if(x==rectangle.x && y==rectangle.y && width==rectangle.width && height==rectangle.height) {
				return true;
			}
			return false;
		}
		
		constexpr bool operator==(const Rectangle<T>& rectangle) const {
			return equals(rectangle);
		}
		
		constexpr bool operator!=(const Rectangle<T>& rectangle) const {
			return !equals(rectangle);
		}
		
		/*! Calculates whether a given point is inside the bounds of the rectangle.
			\param point the point to check
			\returns true if the given point is inside the rectangle's bounds, or false if otherwise*/
		constexpr bool contains(const Vector2<T>& point) const {
			if(point.x >= x && point.y >= y && point.x <= (x + width) && point.y <= (y + height)) {
				return true;
			}
			return false;
		}
		
		/*! Calculates whether a given rectangle is inside the bounds of this rectangle.
			\param rect the rectangle to check
			\returns true if the given rectangle is inside this rectangle's bounds, or false if otherwise*/
		constexpr bool contains(const Rectangle<T>& rect) const  {
			if(x <= rect.x && y <= rect.y && (x + width) >= (rect.x + rect.width) && (y + height) >= (rect.y + rect.height)) {
				return true;
			}
			return false;
		}
		
		/*! Calculates whether a given rectangle is intersecting the bounds of this rectangle.
			\param rect the rectangle to check
			\returns true if the given rectangle is intersecting this rectangle's bounds, or false if otherwise*/
		constexpr bool intersects(const Rectangle<T>& rect) const {
			if (getBottom() <= rect.getTop()) {
				return false;
			}
			if (getTop() >= rect.getBottom()) {
				return false;
			}
			if (getRight() <= rect.getLeft()) {
				return false;
			}
			if (getLeft() >= rect.getRight()) {
				return false;
			}
			return true;
		}
		
		/*! Calculates the area of the rectangle (width*height).
			\returns a value representing the width multiplied by the height*/
		constexpr T getArea() const {
			return width * height;
		}

		/*! Gets an array of the 4 edges in the rectangle
			\returns an ArrayList of Line objects */
		inline ArrayList<Line<T>> getEdges() const {
			return {
				getTopEdge(),
				getRightEdge(),
				getBottomEdge(),
				getLeftEdge()
			};
		}
		
		/*! Gets a rectangle of the overlap between this rectangle and a given rectangle
			\param rect the rectangle to check the intersect with
			\returns a rectangle representing the intersect, or a Rectangle containing an x, y, width, and height of 0 if the rectangle is not intersecting*/
		constexpr Rectangle getIntersect(const Rectangle<T>& rect) const {
			T overlapLeft = 0;
			T overlapTop = 0;
			T overlapRight = 0;
			T overlapBottom = 0;
		
			if(intersects(rect)) {
				if(x <= rect.x) {
					if((x + width) <= (rect.x + rect.width)) {
						overlapLeft = rect.x - x;
						overlapRight = width;
					}
					else {
						overlapLeft = rect.x - x;
						overlapRight = overlapLeft + rect.width;
					}
				}
				else {
					if((x + width) <= (rect.x + rect.width)) {
						overlapLeft = 0;
						overlapRight = width;
					}
					else {
						overlapLeft = 0;
						overlapRight = (rect.x + rect.width) - x;
					}
				}
				
				if(y <= rect.y) {
					if((y + height) <= (rect.y + rect.height)) {
						overlapTop = rect.y - y;
						overlapBottom = height;
					}
					else {
						overlapTop = rect.y - y;
						overlapBottom = overlapTop + rect.height;
					}
				}
				else {
					if((y + height)<= (rect.y + rect.height)) {
						overlapTop = 0;
						overlapBottom = height;
					}
					else {
						overlapTop = 0;
						overlapBottom = (rect.y + rect.height) - y;
					}
				}
			}
			else {
				overlapLeft = 0;
				overlapTop = 0;
				overlapRight = 0;
				overlapBottom = 0;
			}
			
			Rectangle<T> overlapRect;
			overlapRect.x = x + overlapLeft;
			overlapRect.y = y + overlapTop;
			overlapRect.width = overlapRight - overlapLeft;
			overlapRect.height = overlapBottom - overlapTop;
			return overlapRect;
		}
	
		/*! expands this rectangle to contain the given rectangle, as well as itself
			\param rect the rectangle to combine with */
		constexpr void combine(const Rectangle<T>& rect) {
			T rect_left = rect.x;
			T rect_top = rect.y;
			T rect_right = rect.x + rect.width;
			T rect_bottom = rect.y + rect.height;
		
			T left = x;
			T top = y;
			T right = x + width;
			T bottom = y + height;
		
			if(rect_left < left) {
				left = rect_left;
			}
			if(rect_top < top) {
				top = rect_top;
			}
			if(rect_right > right) {
				right = rect_right;
			}
			if(rect_bottom > bottom) {
				bottom = rect_bottom;
			}
		
			x = left;
			y = top;
			width = right-left;
			height = bottom-top;
		}
		
		constexpr Rectangle<T> combined(const Rectangle<T>& rect) const {
			auto newRect = *this;
			newRect.combine(rect);
			return newRect;
		}
		
		/*! Resizes and moves this rectangle, maintaining its aspect ratio, to fit within a given containing rectangle.
			\param container a containing rectangle*/
		constexpr void scaleToFit(const Rectangle<T>&container) {
			T fixedHeight = height;
			long double ratX = ((long double)container.width)/((long double)width);
			fixedHeight = (T)(((long double)height)*ratX);
			
			if(fixedHeight<=container.height) {
				width = container.width;
				height = fixedHeight;
				x = container.x + ((container.width - width)/2);
				y = container.y + ((container.height - height)/2);
				return;
			}
		
			T fixedWidth = width;
			long double ratY = ((long double)container.height)/((long double)height);
			fixedWidth = (T)(((long double)width)*ratY);

			width = fixedWidth;
			height = container.height;
			x = container.x + ((container.width - width)/2);
			y = container.y + ((container.height - height)/2);
		}
		
		/*! Resizes and moves this rectangle, maintaining its aspect ratio, to exactly fill a given containing rectangle.
			\param container a containing rectangle*/
		constexpr void scaleToFill(const Rectangle<T>&container) {
			T fixedHeight = height;
			long double ratX = ((long double)container.width)/((long double)width);
			fixedHeight = (T)(((long double)height)*ratX);
			
			if(fixedHeight>=container.height) {
				width = container.width;
				height = fixedHeight;
				x = container.x + ((container.width - width)/2);
				y = container.y + ((container.height - height)/2);
				return;
			}
			
			T fixedWidth = width;
			double ratY = ((long double)container.height)/((long double)height);
			fixedWidth = (T)(((long double)width)*ratY);

			width = fixedWidth;
			height = container.height;
			x = container.x + ((container.width - width)/2);
			y = container.y + ((container.height - height)/2);
		}
		
		/*! Calculates the center of the rectangle (x+width/2, y+height/2)
			\returns a Vector2 point with the coordinates of the center of the rectangle*/
		constexpr Vector2<T> getCenter() const {
			return Vector2<T>(x+(width/2), y+(height/2));
		}
		
		/*! Calculates the center x coordinate of the rectangle (x+width/2)
			\returns the x coordinate of the center of the rectangle*/
		constexpr T getCenterX() const {
			return x+(width/2);
		}
		
		/*! Calculates the center y coordinate of the rectangle (y+height/2)
			\returns the y coordinate of the center of the rectangle*/
		constexpr T getCenterY() const {
			return y+(height/2);
		}
		
		/*! Gives the top left corner of this rectangle
			\returns a Vector2 representing the point of the top left corner of this rectangle */
		constexpr Vector2<T> getTopLeft() const {
			return Vector2<T>(x, y);
		}
		
		/*! Gives the top right corner of this rectangle
			\returns a Vector2 representing the point of the top right corner of this rectangle */
		constexpr Vector2<T> getTopRight() const {
			return Vector2<T>(x+width, y);
		}
		
		/*! Gives the top center coordinate of this rectangle
			\returns a Vector2 representing the point of the top center of this rectangle */
		constexpr Vector2<T> getTopCenter() const {
			return Vector2<T>(x+(width/2), y);
		}
		
		/*! Gives the bottom left corner of this rectangle
			\returns a Vector2 representing the point of the bottom left corner of this rectangle */
		constexpr Vector2<T> getBottomLeft() const {
			return Vector2<T>(x, y+height);
		}
		
		/*! Gives the bottom right corner of this rectangle
			\returns a Vector2 representing the point of the bottom right corner of this rectangle */
		constexpr Vector2<T> getBottomRight() const {
			return Vector2<T>(x+width, y+height);
		}
		
		/*! Gives the bottom center coordinate of this rectangle
			\returns a Vector2 representing the point of the bottom center of this rectangle */
		constexpr Vector2<T> getBottomCenter() const {
			return Vector2<T>(x+(width/2), y+height);
		}
		
		/*! Gives the center coordinate of the left side of this rectangle
			\returns a Vector2 representing the point of the center of the left side of this rectangle */
		constexpr Vector2<T> getLeftCenter() const {
			return Vector2<T>(x, y+(height/2));
		}
		
		/*! Gives the center coordinate of the right side of this rectangle
			\returns a Vector2 representing the point of the center of the right side of this rectangle */
		constexpr Vector2<T> getRightCenter() const {
			return Vector2<T>(x+width, y+(height/2));
		}

		/*! Gives the left x coordinate of the rectangle */
		constexpr T getLeft() const {
			return x;
		}

		/*! Gives the top y coordinate of the rectangle */
		constexpr T getTop() const {
			return y;
		}

		/*! Gives the right x coordinate of the rectangle */
		constexpr T getRight() const {
			return x + width;
		}

		/*! Gives the bottom y coordinate of the rectangle */
		constexpr T getBottom() const {
			return y + height;
		}
		
		/*! Gives the lefy edge line of the rectangle */
		constexpr Line<T> getLeftEdge() const {
			return Line<T>(getBottomLeft(), getTopLeft());
		}
		
		/*! Gives the top edge line of the rectangle */
		constexpr Line<T> getTopEdge() const {
			return Line<T>(getTopLeft(), getTopRight());
		}
		
		/*! Gives the right edge line of the rectangle */
		constexpr Line<T> getRightEdge() const {
			return Line<T>(getTopRight(), getBottomRight());
		}
		
		/*! Gives the bottom edge line of the rectangle */
		constexpr Line<T> getBottomEdge() const {
			return Line<T>(getBottomRight(), getBottomLeft());
		}
		
		/*! Gives the range of x coordinates covered by the rectangle */
		constexpr Range<T> getXRange() const {
			return Range<T>(x, width);
		}
		
		/*! Gives the range of y coordinates covered by the rectangle */
		constexpr Range<T> getYRange() const {
			return Range<T>(y, height);
		}
		
		/*! Gives the size (width and height) of the Rectangle */
		constexpr Vector2<T> getSize() const {
			return Vector2<T>(width, height);
		}
		
		/*! Translates the rectangle by the given vector
			\param offset the amount to translate the (x,y) coordinates of the rectangle */
		constexpr void translate(const Vector2<T>& offset) {
			x += offset.x;
			y += offset.y;
		}
		
		/*! Creates a new polygon from this polygon, translated by the given vector
			\param offset the amount to translate each point in the new polygon
			\returns the translated polygon */
		constexpr Rectangle<T> translated(const Vector2<T>& offset) const {
			return Rectangle<T>(x+offset.x, y+offset.y, width, height);
		}
		
		/*void scale(double scaleX, double scaleY, const T&originX, const T& originY)
		{
			Transform<T> transform;
			transform.scale((double)scaleX, (double)scaleY, (double)originX, (double)originY);
			Polygon<T> polygon = toPolygon();
			polygon = transform.transform(polygon);
			x = (T)(polygon.getPoints().get(0).x);
			y = (T)(polygon.getPoints().get(0).y);
			width = (T)(polygon.getPoints().get(2).x - polygon.getPoints().get(0).x);
			height = (T)(polygon.getPoints().get(2).y - polygon.getPoints().get(0).y);
		}*/
		
		/*! Creates a polygon with the coordinates of the 4 corners of this rectangle.
			\returns a Polygon object*/
		Polygon<T> toPolygon() const {
			Polygon<T> polygon;
			polygon.addPoint((T)x, (T)y);
			polygon.addPoint((T)(x+width), (T)y);
			polygon.addPoint((T)(x+width), (T)(y+height));
			polygon.addPoint((T)x, (T)(y+height));
			return polygon;
		}
		
		String toString() const {
			return "Rectangle(x:"+fgl::stringify<T>(x)+", y:"+fgl::stringify<T>(y)+
				", width:"+fgl::stringify<T>(width)+", height:"+fgl::stringify<T>(height)+")";
		}
	};
	
	typedef Rectangle<int> RectangleI;
	typedef Rectangle<unsigned int> RectangleU;
	typedef Rectangle<float> RectangleF;
	typedef Rectangle<double> RectangleD;
	
	
	template<typename T>
	Rectangle<T> Polygon<T>::getRectangle() const
	{
		if(points.size() == 0)
		{
			return Rectangle<T>();
		}
		
		T top = points[0].y;
		T bottom = top;
		T left = points[0].x;
		T right = left;
		
		for(size_t i=1; i<points.size(); i++)
		{
			auto& point = points[i];
			if(point.x < left)
			{
				left = point.x;
			}
			else if(point.x > right)
			{
				right = point.x;
			}
			if(point.y < top)
			{
				top = point.y;
			}
			else if(point.y > bottom)
			{
				bottom = point.y;
			}
		}
		return Rectangle<T>(left, top, right-left, bottom-top);
	}
	
	
	template<typename T>
	/*! Contains data about a rectangle.
	Similar to fgl::Rectangle, but rather than having a width and a height, it contains coordinates for top left and bottom right.*/
	class Rect
	{
	public:
		/*! the x coordinate of the left side of the rectangle*/
		T left;
		/*! the y coordinate of the top side of the rectangle*/
		T top;
		/*! the x coordinate of the right side of the rectangle*/
		T right;
		/*! the y coordinate of the bottom side of the rectangle*/
		T bottom;
		
		/*! */
		Rect() : left(0), top(0), right(0), bottom(0)
		{
			//
		}
		
		Rect(const T&left, const T&top, const T&right, const T&bottom) : left(left), top(top), right(right), bottom(bottom)
		{
			//
		}
		
		Rect(const Rect<T>&rect) : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
		{
			//
		}
		
		Rect(const Rectangle<T>&rect) : left(rect.x), top(rect.y), right(rect.x+rect.width), bottom(rect.y+rect.height)
		{
			//
		}
		
		Rect<T>& operator=(const Rect<T>&rect)
		{
			left = rect.left;
			top = rect.top;
			right = rect.right;
			bottom = rect.bottom;
			return *this;
		}
		
		Rectangle<T> toRectangle() const
		{
			return Rectangle<T>(left,top,right-left,bottom-top);
		}
		
		operator Rectangle<T>() const
		{
			return Rectangle<T>(left,top,right-left,bottom-top);
		}
	};
	
	typedef Rect<int> RectI;
	typedef Rect<unsigned int> RectU;
	typedef Rect<float> RectF;
	typedef Rect<double> RectD;
}
