
#pragma once

#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include "Color.hpp"

namespace fgl
{
	/*! Holds an array of pixels, representing an image. An Image must be converted into a TextureImage in order to be drawn to the Window.*/
	class Image
	{
	public:
		/*! default constructor*/
		Image();
		/*! copy constructor*/
		Image(const Image&);
		/*! assignment operator*/
		Image& operator=(const Image&);
		
		
		/*! Creates the image and fills it with a given color.
			\param width the width of the image, in pixels
			\param height the height of the image, in pixels
			\param color the color to fill the image*/
		void create(size_t width, size_t height, const Color&color = Colors::TRANSPARENT);
		/*! Resizes the image and fills any newly created space with a given color*
			\param width the width of the image, in pixels
			\param height the height of the image, in pixels
			\param color the color to fill the empty space with, if any */
		void resize(size_t width, size_t height, const Color& color = Colors::TRANSPARENT);
		/*! Clears the current image and sets the size to (0,0)*/
		void clear();
		
		
		/*! Loads the image data from a pointer
			\param pointer the memory address of the image data
			\param length the length of the image data, in bytes
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromPointer(const void* pointer, size_t length, String* error=nullptr);
		/*! Loads the image data from a file path.
			\param path the path to the image file
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromPath(const String& path, String* error=nullptr);
		/*! Loads the image data from a FILE pointer.
			\param file the FILE pointer to load from
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromFile(FILE* file, String* error=nullptr);
		/*! Saves the image data to a file. File type is automatically deduced from the file extension. This function currently only supports BMP and PNG.
			\param path the path to save the file
			\param error an optional String pointer to store the error message if the function fails
			\return true if the load succeeds, or false if an error occurs */
		bool saveToPath(const String& path, String* error=nullptr) const;
		
		
		/*! Sets the color of the pixel at the given index.
			\throws ImageOutOfBoundsException if the index is outside the bounds of the image
			\param index the index of the pixel
			\param color the color to set the pixel*/
		void setPixel(size_t index, const Color& color);
		/*! Sets the color of the pixel at the given coordinates.
			\throws ImageOutOfBoundsException if the coordinates are outside the bounds of the image
			\param x the x coordinate of the pixel
			\param y the y coordinate of the pixel
			\param color the color to set the pixel*/
		void setPixel(size_t x, size_t y, const Color& color);
		/*! Gets the value of a pixel at the given index.
			\throws ImageOutOfBoundsException if the index is outside the bounds of the image
			\param index the index of the pixel
			\returns a const Color reference representing the value of the pixel*/
		const Color& getPixel(size_t index) const;
		/*! Gets the value of a pixel at the given index.
			\throws ImageOutOfBoundsException if the coordinates are outside the bounds of the image
			\param x the x coordinate of the pixel
			\param y the y coordinate of the pixel
			\returns a const Color reference representing the value of the pixel*/
		const Color& getPixel(size_t x, size_t y) const;
		/*! Gets a container of all the pixels in the image.
			\returns a const ArrayList<Color> reference storing all the pixels in the image*/
		const ArrayList<Color>& getPixels() const;
		
		
		/*! Replaces specified pixels in the image.
			\param colorSwaps an ArrayList containing pairs of Color values, the first in the pair being the original pixel Color, and the second being the replacement*/
		void recolor(const ArrayList<std::pair<Color,Color>>& colorSwaps);
		/*! Applies a composite mask to the image. \see fgl::Color::composite(const Color&)const*/
		void applyCompositeMask(const Image& mask);
		
		
		/*! Gets the total length of the image data (width * height).
			\returns an unsigned integer representing the total number of pixels in the image*/
		size_t getLength() const;
		/*! Gets the width of the image.
			\returns an unsigned integer representing the width of the image, in pixels*/
		size_t getWidth() const;
		/*! Gets the height of the image.
			\returns an unsigned integer representing the height of the image, in pixels*/
		size_t getHeight() const;
		/*! Gets the dimensions of the image.
			\returns a Vector2u with the width (x) and height (y) of the image, in pixels*/
		fgl::Vector2u getDimensions() const;
		
	private:
		ArrayList<Color> pixels;
		size_t width;
		size_t height;
	};
}
