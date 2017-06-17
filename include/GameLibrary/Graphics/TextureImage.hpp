
#pragma once

#include <vector>
#include "Image.hpp"
#include "Graphics.hpp"
#include <GameLibrary/Utilities/Geometry/Polygon.hpp>

namespace fgl
{
	/*! A Image that can be drawn to a Window using the Window's Graphics object. This class is non-copyable and lives on the video card memory.*/
	class TextureImage
	{
		friend class Graphics;
	public:
		/*! default constructor*/
		TextureImage();
		/*! destructor*/
		~TextureImage();
		
		TextureImage(const TextureImage&) = delete;
		TextureImage& operator=(const TextureImage&) = delete;
		
		
		/*! Creates the texture for a specific Window using a Graphics object
			\param width the width of the image, in pixels
			\param height the height of the image, in pixels
			\param graphics the graphics object to create the texture on the video card memory*/
		void create(unsigned int width, unsigned int height, Graphics& graphics);
		/*! Updates the texture using an array of pixels.
			\param pixels an array of pixels to write to the texture. The size of this buffer should be the image width*height*4 bytes
			\note updating a texture is a slow operation, and requires writing to the video card memory*/
		void update(const Color* pixels);
		/*! Updates the texture using an array of pixels.
			\param pixels an array of pixels to write to the texture. The size of this buffer should be region.width*region.height*4 bytes
			\note updating a texture is a slow operation, and requires writing to the video card memory*/
		void update(const Color* pixels, const RectangleU& region);
		/*! Clears all data from the texture and resets the width and height to 0.*/
		void clear();
		
		/*! Loads the image data from a pointer and writes it to the video card memory.
			\param pointer the memory address of the image data
			\param length the length of the image data, in bytes
			\param graphics the graphics object to create the texture on the video card memory
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromPointer(const void* pointer, size_t length, Graphics& graphics, String* error=nullptr);
		/*! Loads the image data from a file path and writes it to the video card memory.
			\param path the path to the image file
			\param graphics the graphics object to create the texture on the video card memory
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromPath(const String& path, Graphics& graphics, String* error=nullptr);
		/*! Loads the image data from a FILE pointer and writes it to the video card memory.
			\param file the FILE pointer to load from
			\param graphics the graphics object to create the texture on the video card memory
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromFile(FILE* file, Graphics& graphics, String* error=nullptr);
		/*! Loads the image data from an Image object and writes it to the video card memory.
			\param image the Image to load from
			\param graphics the graphics object to create the texture on the video card memory
			\param error an optional String pointer to store the error message if the function fails
			\returns true if the load succeeds, or false if an error occurs */
		bool loadFromImage(const Image& image, Graphics& graphics, String* error=nullptr);
		//Image copyToImage() const;
		
		
		/*! Checks a bit array to see if the pixel at the given index is fully transparent or not.
			\param index the index of the pixel
			\returns true if the pixel is visible, and false if the pixel is fully transparent*/
		bool checkPixel(unsigned int index) const;
		/*! Checks a bit array to see if the pixel at the given coordinate is fully transparent or not.
			\param x the x coordinate of the pixel
			\param y the y coordinate of the pixel
			\returns true if the pixel is visible, and false if the pixel is fully transparent*/
		bool checkPixel(unsigned int x, unsigned int y) const;
		/*! Gets a bit vector storing each pixel's transparency state, true for visible and false for transparent.
			\returns a const std::vector<bool> reference containing all the pixel visibility states*/
		const std::vector<bool>& getPixelBools() const;
		
		
		/*! Gets the total size of the texture (width * height).
			\returns an unsigned integer representing the total number of pixels in the texture*/
		unsigned int getSize() const;
		/*! Gets the width of the texture.
			\returns an unsigned integer representing the width of the texture, in pixels*/
		unsigned int getWidth() const;
		/*! Gets the height of the texture.
			\returns an unsigned integer representing the height of the texture, in pixels*/
		unsigned int getHeight() const;
		/*! Gets the dimensions of the texture.
			\returns a Vector2u with the width (x) and height (y) of the texture, in pixels*/
		Vector2u getDimensions() const;

		/*! Traces a polygon outline for the texture
			\returns a PolygonD representing the outline of the texture */
		PolygonD traceOutline() const;
		
	private:
		void* texture;
		std::vector<bool> pixels;
		unsigned int width;
		unsigned int height;
	};
}
