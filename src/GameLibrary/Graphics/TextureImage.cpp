
#include <GameLibrary/Graphics/TextureImage.hpp>
#include <GameLibrary/Graphics/Image.hpp>
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.hpp>
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.hpp>
#include <GameLibrary/Exception/Graphics/TextureImageUpdateException.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include "../SDL_ext/SDL_RWops_ext.hpp"

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
	
	void TextureImage::create(size_t w, size_t h, Graphics&graphics)
	{
		if(w>0 && h>0)
		{
			//TODO check for integer overflow
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
			SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
			texture = (void*)newTexture;
			width = w;
			height = h;
			//TODO check for integer overflow
			size_t total = w*h;
			pixels.resize(total);
			for(size_t i=0; i<total; i++)
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

		//TODO check for integer overflow
		size_t w = (size_t)surface->w;
		size_t h = (size_t)surface->h;
		size_t totalSize = w*h;
		
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

		unsigned int pitchDif = ((unsigned int)surface->pitch - ((unsigned int)w*bpp));

		size_t counter = 0;
		byte*surfacePixels = (byte*)surface->pixels;

		size_t i=0;
		for(size_t ycnt=0; ycnt<h; ycnt++)
		{
			for(size_t xcnt = 0; xcnt < w; xcnt++)
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
						Uint32 color = *((Uint32*)&surfacePixels[counter]);
						px.r = (byte)((color & rmask) >> rshift);
						px.g = (byte)((color & gmask) >> gshift);
						px.b = (byte)((color & bmask) >> bshift);
						px.a = 255;
					}
					break;

					case 4:
					{
						Uint32 color = *((Uint32*)&surfacePixels[counter]);
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
				counter += (size_t)bpp;
			}
			counter += (size_t)pitchDif;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		if(mustlock != 0)
		{
			SDL_UnlockSurface(surface);
		}
		SDL_FreeSurface(surface);

		return texture;
	}

	bool TextureImage::loadFromPointer(const void* pointer, size_t length, Graphics& graphics, String* error)
	{
		//TODO check for integer overflow
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(pointer, (int)length), 1);
		if(surface != nullptr)
		{
			size_t w = (size_t)surface->w;
			size_t h = (size_t)surface->h;
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
	
	bool TextureImage::loadFromPath(const String& path, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load(path);
		if(surface != nullptr)
		{
			size_t w = (size_t)surface->w;
			size_t h = (size_t)surface->h;
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
	
	bool TextureImage::loadFromFile(FILE* file, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromFILE(file, SDL_FALSE), 1);
		if(surface != nullptr)
		{
			size_t w = (size_t)surface->w;
			size_t h = (size_t)surface->h;
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

	bool TextureImage::loadFromImage(const Image& image, Graphics& graphics, String*error)
	{
		const ArrayList<Color>& image_pixels = image.getPixels();
		if(image_pixels.size()>0)
		{
			//TODO check for integer overflow
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
			//TODO check for integer overflow
			size_t w = image.getWidth();
			size_t h = image.getHeight();
			size_t totalsize = w*h;
			void* pixelptr;
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

			fgl::Uint32* texture_pixels = (Uint32*)pixelptr;
			for(size_t i=0; i<totalsize; i++)
			{
				const Color& px = image_pixels[i];
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

	bool TextureImage::checkPixel(size_t index) const
	{
		if(index < pixels.size())
		{
			return pixels[index];
		}
		throw ImageOutOfBoundsException(index,width,height);
	}

	bool TextureImage::checkPixel(size_t x, size_t y) const
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

	size_t TextureImage::getLength() const
	{
		return pixels.size();
	}

	size_t TextureImage::getWidth() const
	{
		return width;
	}

	size_t TextureImage::getHeight() const
	{
		return height;
	}

	Vector2u TextureImage::getDimensions() const
	{
		return Vector2u((unsigned int)width, (unsigned int)height);
	}

	PolygonD TextureImage::traceOutline() const
	{
		return traceOutline(RectangleU(0, 0, (unsigned int)width, (unsigned int)height));
	}

	PolygonD TextureImage::traceOutline(const RectangleU& sourceRect) const
	{
		auto leftBarrier = sourceRect.x;
		auto rightBarrier = sourceRect.x+sourceRect.width;
		auto topBarrier = sourceRect.y;
		auto bottomBarrier = sourceRect.y + sourceRect.height;
		if(leftBarrier > width)
		{
			throw ImageOutOfBoundsException((size_t)leftBarrier, (size_t)sourceRect.y, (size_t)width, (size_t)height);
		}
		else if(rightBarrier > width)
		{
			throw ImageOutOfBoundsException((size_t)rightBarrier, (size_t)sourceRect.y, (size_t)width, (size_t)height);
		}
		else if(topBarrier > height)
		{
			throw ImageOutOfBoundsException((size_t)sourceRect.x, (size_t)topBarrier, (size_t)width, (size_t)height);
		}
		else if(bottomBarrier > height)
		{
			throw ImageOutOfBoundsException((size_t)sourceRect.x, (size_t)bottomBarrier, (size_t)width, (size_t)height);
		}

		//NOTE: credit to gameEditor for polygon image tracing code
		PolygonD outline;
		size_t line = (width*topBarrier);
		size_t leftCount = 0;
		for(size_t y=0; y<sourceRect.height; y++)
		{
			// since it's going to be a convex hull,
			// scanning a left and right edge should suffice
			double left = -1, right = -1;

			for(size_t x=0; x<sourceRect.width; x++)
			{
				if(pixels[line+leftBarrier+x])
				{
					left = (double)x;
					break;
				}
			}

			for(size_t x=(width-1); x!=-1; x--)
			{
				if(pixels[line+leftBarrier+x])
				{
					right = (double)x + 1;
					break;
				}
			}

			// This is tricky. The array should look like:
			// (l1, l2, l3, l4, r4, r3, r2, r1)
			// where 'l' is a left node, 'r' is a right node
			// the indices correspond to y coordinates

			// leftCount as an index is the position of the first r element
			// (or where the r should be inserted)
			if(left >= 0)
			{
				// (l1, l2, l3, l4, _l5_, r4, r3, r2, r1)
				outline.addPoint(leftCount, Vector2d(left, (double)y));
				// leftCounts points at r4
				leftCount++;
			}

			if(right >= 0)
			{
				// (l1, l2, l3, l4, l5, _r5_, r4, r3, r2, r1)
				outline.addPoint(leftCount, Vector2d(right, (double)y));
			}

			line += (size_t)width;
		}
		outline.removeRedundancies();
		return outline;
	}
}
