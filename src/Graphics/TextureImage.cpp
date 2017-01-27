
#include <GameLibrary/Graphics/TextureImage.hpp>
#include <GameLibrary/Graphics/Image.hpp>
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.hpp>
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.hpp>
#include <GameLibrary/Exception/Graphics/TextureImageUpdateException.hpp>
#include "SDL.h"
#include "SDL_image.h"

namespace fgl
{
	TextureImage::TextureImage()
	{
		int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
		if(IMG_Init(flags) != flags)
		{
			throw InitializeLibraryException("SDL_image", IMG_GetError());
		}

		texture = nullptr;
		width = 0;
		height = 0;
	}

	TextureImage::~TextureImage()
	{
		if(texture != nullptr)
		{
			SDL_DestroyTexture((SDL_Texture*)texture);
		}
	}
	
	void TextureImage::create(unsigned int w, unsigned int h, Graphics&graphics)
	{
		if(w>0 && h>0)
		{
			SDL_Texture* newTexture = SDL_CreateTexture((SDL_Renderer*)graphics.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, (int)w, (int)h);
			if(newTexture == nullptr)
			{
				throw TextureImageCreateException(SDL_GetError());
			}
			if(texture != nullptr)
			{
				SDL_DestroyTexture((SDL_Texture*)texture);
				texture = nullptr;
			}
			texture = (void*)newTexture;
			width = w;
			height = h;
			unsigned int total = w*h;
			pixels.resize(total);
			for(unsigned int i=0; i<total; i++)
			{
				pixels[i] = false;
			}
		}
		else
		{
			if(texture != nullptr)
			{
				SDL_DestroyTexture((SDL_Texture*)texture);
				texture = nullptr;
			}
			width = 0;
			height = 0;
			pixels.resize(0);
			pixels.shrink_to_fit();
		}
	}

	void TextureImage::update(const Color* pixels)
	{
		if(texture == nullptr)
		{
			throw TextureImageUpdateException("Cannot update an empty TextureImage");
		}
		SDL_UpdateTexture((SDL_Texture*)texture, nullptr, pixels, (int)(width*4));
	}

	void TextureImage::update(const Color* pixels, const RectangleU& region)
	{
		if(texture == nullptr)
		{
			throw TextureImageUpdateException("Cannot update an empty TextureImage");
		}
		else if(region.x >= width || (region.x+region.width) >= width || region.y >= height || (region.y+region.height) >= height)
		{
			throw OutOfBoundsException("region is out of bounds");
		}

		SDL_Rect rect;
		rect.x = (int)region.x;
		rect.y = (int)region.y;
		rect.w = (int)region.width;
		rect.h = (int)region.height;

		SDL_UpdateTexture((SDL_Texture*)texture, &rect, pixels, (int)(width*4));
	}
	
	void TextureImage::clear()
	{
		if(texture != nullptr)
		{
			SDL_DestroyTexture((SDL_Texture*)texture);
			texture = nullptr;
		}
		width = 0;
		height = 0;
	}

	SDL_Texture* TextureImage_loadFromSDLSurface(SDL_Surface* surface, std::vector<bool>& pixels, SDL_Renderer* renderer, String* error, bool freeSurface)
	{
		SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
		if(convertedSurface==nullptr)
		{
			if(error!=nullptr)
			{
				*error = SDL_GetError();
			}
			if(freeSurface)
			{
				SDL_FreeSurface(surface);
			}
			return nullptr;
		}
		if(freeSurface)
		{
			SDL_FreeSurface(surface);
		}
		surface = convertedSurface;

		unsigned int w = (unsigned int)surface->w;
		unsigned int h = (unsigned int)surface->h;
		unsigned int totalSize = w*h;
		
		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, (int)w, (int)h);
		if(texture == nullptr)
		{
			if(error!=nullptr)
			{
				*error = SDL_GetError();
			}
			SDL_FreeSurface(surface);
			return nullptr;
		}

		int mustlock = SDL_MUSTLOCK(surface);
		if(mustlock!=0)
		{
			if(SDL_LockSurface(surface) < 0)
			{
				if(error!=nullptr)
				{
					*error = SDL_GetError();
				}
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(convertedSurface);
				return nullptr;
			}
		}
		
		SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
		
		unsigned int bpp = (unsigned int)surface->format->BytesPerPixel;
		
		unsigned int rmask = (unsigned int)surface->format->Rmask;
		unsigned int rshift = (unsigned int)surface->format->Rshift;
		unsigned int gmask = (unsigned int)surface->format->Gmask;
		unsigned int gshift = (unsigned int)surface->format->Gshift;
		unsigned int bmask = (unsigned int)surface->format->Bmask;
		unsigned int bshift = (unsigned int)surface->format->Bshift;
		unsigned int amask = (unsigned int)surface->format->Amask;
		unsigned int ashift = (unsigned int)surface->format->Ashift;

		pixels.resize(totalSize);
		pixels.shrink_to_fit();

		unsigned int pitchDif = ((unsigned int)surface->pitch - (w*bpp));

		unsigned int counter = 0;
		byte*surfacePixels = (byte*)surface->pixels;

		unsigned int i=0;
		for(unsigned int ycnt=0; ycnt<h; ycnt++)
		{
			for(unsigned int xcnt = 0; xcnt < w; xcnt++)
			{
				Color px;
				switch(bpp)
				{
					case 1:
					px.r = surfacePixels[counter];
					px.g = surfacePixels[counter];
					px.b = surfacePixels[counter];
					px.a = 255;
					break;

					case 2:
					px.r = surfacePixels[counter];
					px.g = surfacePixels[counter+1];
					px.b = surfacePixels[counter+1];
					px.a = 255;
					break;

					case 3:
					{
						int color = *((int*)&surfacePixels[counter]);
						px.r = (byte)((color & rmask) >> rshift);
						px.g = (byte)((color & gmask) >> gshift);
						px.b = (byte)((color & bmask) >> bshift);
						px.a = 255;
					}
					break;

					case 4:
					{
						int color = *((int*)&surfacePixels[counter]);
						px.r = (byte)((color & rmask) >> rshift);
						px.g = (byte)((color & gmask) >> gshift);
						px.b = (byte)((color & bmask) >> bshift);
						px.a = (byte)((color & amask) >> ashift);
					}
					break;
				}

				if(px.a>0)
				{
					pixels[i] = true;
				}
				else
				{
					pixels[i] = false;
				}
				i++;
				counter += bpp;
			}
			counter += pitchDif;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		if(mustlock != 0)
		{
			SDL_UnlockSurface(surface);
		}
		SDL_FreeSurface(surface);

		return texture;
	}

	bool TextureImage::loadFromPointer(const void* pointer, size_t size, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(pointer, (int)size), 1);
		if(surface != nullptr)
		{
			unsigned int w = (unsigned int)surface->w;
			unsigned int h = (unsigned int)surface->h;
			SDL_Texture* newTexture = TextureImage_loadFromSDLSurface(surface, pixels, (SDL_Renderer*)graphics.renderer, error, true);
			if(newTexture!=nullptr)
			{
				width = w;
				height = h;
				if(texture != nullptr)
				{
					SDL_DestroyTexture((SDL_Texture*)texture);
				}
				texture = newTexture;
				return true;
			}
			return false;
		}
		if(error!=nullptr)
		{
			*error = IMG_GetError();
		}
		return false;
	}
	
	bool TextureImage::loadFromFile(const String& path, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load(path);
		if(surface != nullptr)
		{
			unsigned int w = (unsigned int)surface->w;
			unsigned int h = (unsigned int)surface->h;
			SDL_Texture* newTexture = TextureImage_loadFromSDLSurface(surface, pixels, (SDL_Renderer*)graphics.renderer, error, true);
			if(newTexture!=nullptr)
			{
				width = w;
				height = h;
				if(texture != nullptr)
				{
					SDL_DestroyTexture((SDL_Texture*)texture);
				}
				texture = newTexture;
				return true;
			}
			return false;
		}
		if(error!=nullptr)
		{
			*error = IMG_GetError();
		}
		return false;
	}

	bool TextureImage::loadFromImage(const Image&image, Graphics&graphics, String*error)
	{
		const ArrayList<Color>& image_pixels = image.getPixels();
		if(image_pixels.size()>0)
		{
			SDL_Texture* newTexture = SDL_CreateTexture((SDL_Renderer*)graphics.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, (int)image.getWidth(), (int)image.getHeight());
			if(newTexture == nullptr)
			{
				if(error!=nullptr)
				{
					*error = SDL_GetError();
				}
				return false;
			}
			if(texture != nullptr)
			{
				SDL_DestroyTexture((SDL_Texture*)texture);
				texture = nullptr;
			}
			unsigned int w = image.getWidth();
			unsigned int h = image.getHeight();
			unsigned int totalsize = w*h;
			void*pixelptr;
			int pitch;
			if(SDL_LockTexture(newTexture, nullptr, &pixelptr, &pitch) < 0)
			{
				if(error!=nullptr)
				{
					*error = SDL_GetError();
				}
				SDL_DestroyTexture(newTexture);
				return false;
			}

			texture = (void*)newTexture;

			pixels.resize(totalsize);
			pixels.shrink_to_fit();

			int*texture_pixels = (int*)pixelptr;
			for(unsigned int i=0; i<totalsize; i++)
			{
				const Color&px = image_pixels[i];
				if(px.a>0)
				{
					pixels[i] = true;
				}
				else
				{
					pixels[i] = false;
				}
				texture_pixels[i] = px.getRGBA();
			}

			SDL_UnlockTexture((SDL_Texture*)texture);
			SDL_SetTextureBlendMode((SDL_Texture*)texture,SDL_BLENDMODE_BLEND);

			width = w;
			height = h;

			return true;
		}
		else
		{
			if(texture != nullptr)
			{
				SDL_DestroyTexture((SDL_Texture*)texture);
				texture = nullptr;
			}
			width = 0;
			height = 0;
			pixels.resize(0);
			pixels.shrink_to_fit();
			return true;
		}
		return false;
	}

	bool TextureImage::checkPixel(unsigned int index) const
	{
		if(index < pixels.size())
		{
			return pixels[index];
		}
		throw ImageOutOfBoundsException(index,width,height);
	}

	bool TextureImage::checkPixel(unsigned int x, unsigned int y) const
	{
		if(x < width && y < height)
		{
			return pixels[(width*y)+x];
		}
		throw ImageOutOfBoundsException(x, y, width, height);
	}

	const std::vector<bool>& TextureImage::getPixelBools() const
	{
		return pixels;
	}

	unsigned int TextureImage::getSize() const
	{
		return (unsigned int)pixels.size();
	}

	unsigned int TextureImage::getWidth() const
	{
		return width;
	}

	unsigned int TextureImage::getHeight() const
	{
		return height;
	}

	fgl::Vector2u TextureImage::getDimensions() const
	{
		return fgl::Vector2u(width, height);
	}
}
