
#pragma once

#include <GameLibrary/Utilities/Geometry/Rectangle.h>
#include <GameLibrary/Utilities/Geometry/Transform.h>

namespace fgl
{
	/*! Iterates through pixel coordinates in a rectangle at a specified speed. Useful for efficiently detecting pixel level collisions.*/
	class PixelIterator
	{
	public:
		/*! Constructs a PixelIterator to loop through a given area.
			\param dimensions the actual size of the image or canvas, in pixels
			\param srcRect the source rectangle of the area being checked
			\param dstRect the destination rectangle of the section of the canvas or image being drawn; Useful if an image or canvas is stretched
			\param loopRect the rectangle to loop through in terms of destination pixels, rather than source pixels
			\param xincrement the amount to increment the x value through each iteration; Value is in terms of destination pixels, rather than source pixels
			\param yincrement the amount to increment the y value through each row iteration; Value is in terms of destination pixels, rather than source pixels
			\param mirrorHorizontal true if the destination image or canvas is mirrored horizontally, and false if otherwise
			\param mirrorVertical true if the destination image or canvas is mirrored vertically, and false if otherwise
			\throws fgl::IllegalArgumentException if loopRect is not contained within dstRect or if srcRect is stretches larger than dimensions*/
		PixelIterator(const Vector2u&dimensions, const RectangleU&srcRect, const RectangleD&dstRect, const RectangleD&loopRect, double xincrement, double yincrement, bool mirrorHorizontal=false, bool mirrorVertical=false);
		/*! Constructs a PixelIterator to loop through a given area.
			\param dimensions the actual size of the image or canvas, in pixels
			\param srcRect the source rectangle of the area being checked
			\param dstRect the destination rectangle of the section of the canvas or image being drawn; Useful if an image or canvas is stretched
			\param loopRect the rectangle to loop through in terms of destination pixels, rather than source pixels
			\param xincrement the amount to increment the x value through each iteration; Value is in terms of destination pixels, rather than source pixels
			\param yincrement the amount to increment the y value through each row iteration; Value is in terms of destination pixels, rather than source pixels
			\param transform the transformation of the desination image or canvas
			\param ratio the ratio of the source rectangle size over the non-transformed destination rectangle size
			\param mirrorHorizontal true if the destination image or canvas is mirrored horizontally, and false if otherwise
			\param mirrorVertical true if the destination image or canvas is mirrored vertically, and false if otherwise
			\throws fgl::IllegalArgumentException if loopRect is not contained within dstRect or if srcRect is stretches larger than dimensions*/
		PixelIterator(const Vector2u&dimensions, const RectangleU&srcRect, const RectangleD&dstRect, const RectangleD&loopRect, double xincrement, double yincrement, const TransformD&transform, const Vector2d&ratio, bool mirrorHorizontal=false, bool mirrorVertical=false);
		/*! copy constructor*/
		PixelIterator(const PixelIterator&);
		/*! assignment operator*/
		PixelIterator& operator=(const PixelIterator&);
		
		
		/*! iterates to the next pixel "index".
			\returns true if there is still more area to iterate through, or false if the iterator has finished iterating and has returned to the starting point*/
		bool nextPixelIndex();
		/*! Gets the current pixel x and y coordinates, in source image units. These coordinates may be inside or outside of the image source rectangle.
		\returns a Vector2d representing the current pixel coordinates of the iterator*/
		Vector2d getCurrentPixelPoint() const;
		/*! Gets the current pixel "index". This value should be floored and converted to an integer before checking against an image or canvas.
			\returns a double value representing the current iteration through the images pixel indexes; This value will be negative if outside of the bounds of the image or canvas*/
		double getCurrentPixelIndex() const;
		
	private:
		Vector2d calculatePixelPoint();
		double calculatePixelIndex(const Vector2d& pixelPoint);
		
		Vector2d dimensions;
		
		RectangleU srcRect;
		RectangleD srcRectD;
		double srcRectRight;
		double srcRectBottom;
		RectangleD dstRect;
		RectangleD loopRect;
		RectD loopRectRel; //relative to dstRect

		TransformD inverseTransform;
		bool usesTransform;
		bool mirrorHorizontal;
		bool mirrorVertical;
		bool started;
		
		Vector2d currentPoint; //relative to dstRect
		Vector2d currentPixelPoint; //relative to the srcRect
		double currentPixelIndex; //relative to the image
		double lastRowStartIndex;
		double row;
		Vector2d incr;
		Vector2d incrpxl;
		Vector2d ratio; // ratio of srcSize/dstSize
	};
}
