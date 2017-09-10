
#include <GameLibrary/Graphics/Image.hpp>
#include <GameLibrary/Graphics/PixelIterator.hpp>
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.hpp>
#include <GameLibrary/Exception/Graphics/UnsupportedImageFormatException.hpp>
#include <SDL_image.h>
#include "../SDL_ext/SDL_RWops_ext.hpp"

namespace fgl
{
	Image::Image()
	{
		int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
		if(IMG_Init(flags) != flags)
		{
			throw InitializeLibraryException("SDL_image", IMG_GetError());
		}

		width = 0;
		height = 0;
	}

	Image::Image(const Image&img)
	{
		pixels = img.pixels;
		width = img.width;
		height = img.height;
	}
	
	Image& Image::operator=(const Image&img)
	{
		pixels = img.pixels;
		width = img.width;
		height = img.height;
		return *this;
	}
	
	void Image::create(size_t w, size_t h, const Color& color)
	{
		//TODO check for integer overflow
		size_t total = w*h;
		if(width == 0 || height == 0)
		{
			width = 0;
			height = 0;
		}
		else
		{
			width = w;
			height = h;
		}
		pixels.resize(total);
		for(size_t i = 0; i < total; i++)
		{
			pixels[i] = color;
		}
	}
	
	void Image::resize(size_t newWidth, size_t newHeight, const Color& color)
	{
		ArrayList<Color> newPixels;
		//TODO check for integer overflow
		size_t total = newWidth*newHeight;
		newPixels.resize(total);
		
		size_t i=0;
		for(size_t y=0; y<newHeight; y++)
		{
			for(size_t x=0; x<newWidth; x++)
			{
				if(x < width && y < height)
				{
					newPixels[i] = pixels[(width*y)+x];
				}
				else
				{
					newPixels[i] = color;
				}
				i++;
			}
		}
		
		pixels = newPixels;
		width = newWidth;
		height = newHeight;
	}

	void Image::clear()
	{
		pixels.clear();
		width = 0;
		height = 0;
	}

	bool Image_loadFromSDLSurface(SDL_Surface* surface, ArrayList<Color>& pixels, String* error)
	{
		int mustlock = SDL_MUSTLOCK(surface);
		if(mustlock!=0)
		{
			if(SDL_LockSurface(surface) < 0)
			{
				if(error!=nullptr)
				{
					*error = SDL_GetError();
				}
				return false;
			}
		}

		unsigned int bpp = (unsigned int)surface->format->BytesPerPixel;

		unsigned int rmask = (unsigned int)surface->format->Rmask;
		unsigned int rshift = (unsigned int)surface->format->Rshift;
		unsigned int gmask = (unsigned int)surface->format->Gmask;
		unsigned int gshift = (unsigned int)surface->format->Gshift;
		unsigned int bmask = (unsigned int)surface->format->Bmask;
		unsigned int bshift = (unsigned int)surface->format->Bshift;
		unsigned int amask = (unsigned int)surface->format->Amask;
		unsigned int ashift = (unsigned int)surface->format->Ashift;

		//TODO check for integer overflow
		size_t width = (size_t)surface->w;
		size_t height = (size_t)surface->h;
		size_t total = width*height;
		pixels.resize(total);

		unsigned int pitchDif = ((unsigned int)surface->pitch - (width*bpp));

		size_t counter = 0;
		byte*surfacePixels = (byte*)surface->pixels;

		size_t i=0;
		for(size_t ycnt=0; ycnt<height; ycnt++)
		{
			for(size_t xcnt = 0; xcnt < width; xcnt++)
			{
				switch(bpp)
				{
					case 1:
					pixels[i].r = surfacePixels[counter];
					pixels[i].g = surfacePixels[counter];
					pixels[i].b = surfacePixels[counter];
					pixels[i].a = 255;
					break;

					case 2:
					pixels[i].r = surfacePixels[counter];
					pixels[i].g = surfacePixels[counter+1];
					pixels[i].b = surfacePixels[counter+1];
					pixels[i].a = 255;
					break;

					case 3:
					{
						Uint32 color = *((Uint32*)&surfacePixels[counter]);
						pixels[i].r = (byte)((color & rmask) >> rshift);
						pixels[i].g = (byte)((color & gmask) >> gshift);
						pixels[i].b = (byte)((color & bmask) >> bshift);
						pixels[i].a = 255;
					}
					break;

					case 4:
					{
						Uint32 color = *((Uint32*)&surfacePixels[counter]);
						pixels[i].r = (byte)((color & rmask) >> rshift);
						pixels[i].g = (byte)((color & gmask) >> gshift);
						pixels[i].b = (byte)((color & bmask) >> bshift);
						pixels[i].a = (byte)((color & amask) >> ashift);
					}
					break;
				}
				i++;
				counter += (size_t)bpp;
			}
			counter += (size_t)pitchDif;
		}

		if(mustlock != 0)
		{
			SDL_UnlockSurface(surface);
		}

		return true;
	}

	bool Image::loadFromPointer(const void* pointer, size_t length, String* error)
	{
		//TODO check for integer overflow
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(pointer, (int)length), 1);
		if(surface != nullptr)
		{
			if(Image_loadFromSDLSurface(surface, pixels, error))
			{
				width = (size_t)surface->w;
				height = (size_t)surface->h;
				SDL_FreeSurface(surface);
				return true;
			}
			SDL_FreeSurface(surface);
			return false;
		}
		if(error!=nullptr)
		{
			*error = IMG_GetError();
		}
		return false;
	}
	
	bool Image::loadFromPath(const String& path, String* error)
	{
		SDL_Surface* surface = IMG_Load(path);
		if(surface != nullptr)
		{
			if(Image_loadFromSDLSurface(surface, pixels, error))
			{
				width = (size_t)surface->w;
				height = (size_t)surface->h;
				SDL_FreeSurface(surface);
				return true;
			}
			SDL_FreeSurface(surface);
			return false;
		}
		if(error!=nullptr)
		{
			*error = IMG_GetError();
		}
		return false;
	}
	
	bool Image::loadFromFile(FILE* file, String* error)
	{
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromFILE(file, SDL_FALSE), 1);
		if(surface != nullptr)
		{
			if(Image_loadFromSDLSurface(surface, pixels, error))
			{
				width = (size_t)surface->w;
				height = (size_t)surface->h;
				SDL_FreeSurface(surface);
				return true;
			}
			SDL_FreeSurface(surface);
			return false;
		}
		if(error!=nullptr)
		{
			*error = IMG_GetError();
		}
		return false;
	}

	bool Image::saveToPath(const String& path, String* error) const
	{
		size_t dotIndex = path.lastIndexOf('.');
		if(dotIndex == String::NOT_FOUND)
		{
			throw UnsupportedImageFormatException("");
		}
		else
		{
			String format = path.substring(dotIndex + 1, path.length()).toLowerCase();
			if(format.indexOf('/')!=String::NOT_FOUND || format.indexOf('\\')!=String::NOT_FOUND)
			{
				throw UnsupportedImageFormatException("");
			}
			else
			{
				if(format.equals("png"))
				{
					//TODO check for integer overflow
					SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)pixels.getData(), (int)width, (int)height, 32, width*4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
					if(surface == nullptr)
					{
						if(error!=nullptr)
						{
							*error = SDL_GetError();
						}
						return false;
					}

					if(IMG_SavePNG(surface, path) != 0)
					{
						if(error!=nullptr)
						{
							*error = IMG_GetError();
						}
						SDL_FreeSurface(surface);
						return false;
					}

					SDL_FreeSurface(surface);
					return true;
				}
				else if(format.equals("bmp"))
				{
					SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)pixels.getData(), (int)width, (int)height, 32, width*4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
					if(surface == nullptr)
					{
						if(error!=nullptr)
						{
							*error = SDL_GetError();
						}
						return false;
					}

					if(SDL_SaveBMP(surface, path) != 0)
					{
						if(error!=nullptr)
						{
							*error = SDL_GetError();
						}
						SDL_FreeSurface(surface);
						return false;
					}

					SDL_FreeSurface(surface);
					return true;
				}
				else
				{
					throw UnsupportedImageFormatException(format);
				}
			}
		}
		
		return false;
	}
	
	void Image::setPixel(size_t index, const Color& color)
	{
		try
		{
			pixels[index] = color;
		}
		catch(const ArrayListOutOfBoundsException&)
		{
			throw ImageOutOfBoundsException(index, width, height);
		}
	}
	
	void Image::setPixel(size_t x, size_t y, const Color& color)
	{
		try
		{
			pixels[(width*y)+x] = color;
		}
		catch(const ArrayListOutOfBoundsException&)
		{
			throw ImageOutOfBoundsException(x, y, width, height);
		}
	}

	const Color& Image::getPixel(size_t index) const
	{
		try
		{
			return pixels[index];
		}
		catch(const ArrayListOutOfBoundsException&)
		{
			throw ImageOutOfBoundsException(index, width, height);
		}
	}

	const Color& Image::getPixel(size_t x, size_t y) const
	{
		try
		{
			return pixels[(width*y)+x];
		}
		catch(const ArrayListOutOfBoundsException&)
		{
			throw ImageOutOfBoundsException(x, y, width, height);
		}
	}

	const ArrayList<Color>& Image::getPixels() const
	{
		return pixels;
	}

	void Image::recolor(const ArrayList<std::pair<Color, Color>>& colorSwaps)
	{
		size_t total = pixels.size();
		size_t totalSwaps = colorSwaps.size();
		for(size_t i=0; i<total; i++)
		{
			const Color& color = pixels.get(i);
			for(size_t j=0; j<totalSwaps; j++)
			{
				const std::pair<Color,Color>& colorSwap = colorSwaps.get(j);
				if(color.equals(colorSwap.first))
				{
					pixels.set(i, colorSwap.second);
					j = totalSwaps;
				}
			}
		}
	}
	
	void Image::applyCompositeMask(const Image& mask)
	{
		if(width == 0 || height == 0 || mask.width==0 || mask.height==0)
		{
			return;
		}
		RectangleD dstRect = RectangleD(0,0,(double)width,(double)height);
		PixelIterator pxlIter(Vector2u(width,height), RectangleU(0,0,width,height), dstRect, dstRect, 1, 1, false, false);
		
		PixelIterator mask_pxlIter(Vector2u(mask.width,mask.height), RectangleU(0,0,mask.width,mask.height), dstRect, dstRect, 1, 1, false, false);
		
		std::vector<bool> masked(pixels.size());
		size_t total = pixels.size();
		for(size_t i=0; i<total; i++)
		{
			masked[i] = false;
		}

		bool running = pxlIter.nextPixelIndex();
		bool mask_running = mask_pxlIter.nextPixelIndex();
		while(running && mask_running)
		{
			double pxlIndex = pxlIter.getCurrentPixelIndex();
			double mask_pxlIndex = mask_pxlIter.getCurrentPixelIndex();
			
			if(pxlIndex>=0 && mask_pxlIndex>=0)
			{
				unsigned int index = (unsigned int)pxlIndex;
				if(!masked[index])
				{
					unsigned int mask_index = (unsigned int)mask_pxlIndex;
					const Color& mask_color = mask.getPixel(mask_index);
					Color curcol = pixels[index];
					pixels[index] = curcol.composite(mask_color);
					masked[index] = true;
				}
			}
			
			running = pxlIter.nextPixelIndex();
			mask_running = mask_pxlIter.nextPixelIndex();
		}
		if(running != mask_running)
		{
			throw Exception("Unknown masking bug. This exception means there is a bug within the Image::applyMask function");
		}
	}

	size_t Image::getLength() const
	{
		return (size_t)pixels.size();
	}
	
	size_t Image::getWidth() const
	{
		return width;
	}

	size_t Image::getHeight() const
	{
		return height;
	}

	fgl::Vector2u Image::getDimensions() const
	{
		return fgl::Vector2u(width, height);
	}
}
