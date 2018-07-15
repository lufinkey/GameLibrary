
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.hpp>

namespace fgl
{
	/*! Thrown to indicate that a call to Image::getPixel, Image::setPixel, or TextureImage::checkPixel is out of bounds*/
	class ImageOutOfBoundsException : public OutOfBoundsException
	{
	public:
		/*! The out of bounds index*/
		size_t index;
		/*! the x pixel*/
		size_t x;
		/*! the y pixel*/
		size_t y;
		
		/*! the size of the image (width * height)*/
		size_t size;
		/*! the width of the image*/
		size_t width;
		/*! the height of the image*/
		size_t height;
		
		/*! Constructs an exception for an x,y coordinate, and a width and height of an image.
			\param x the x pixel
			\param y the y pixel
			\param width the width of the image
			\param height the height of the image*/
		ImageOutOfBoundsException(size_t x, size_t y, size_t width, size_t height);
		/*! Constructs an exception for the pixel index and a width and height of an image.
			\param index the pixel index
			\param width the width of the image
			\param height the height of the image*/
		ImageOutOfBoundsException(size_t index, size_t width, size_t height);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, ImageOutOfBoundsException, fgl::OutOfBoundsException)
