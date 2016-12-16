
#include <GameLibrary/Graphics/TextureImage.h>
#include <GameLibrary/Graphics/Image.h>
#include <GameLibrary/IO/Console.h>
#include <GameLibrary/Exception/InitializeLibraryException.h>
#include <GameLibrary/Exception/Graphics/ImageOutOfBoundsException.h>
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.h>
#include <GameLibrary/Exception/Graphics/TextureImageUpdateException.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

	void TextureImage::update(const Color*pxls)
	{
		if(texture == nullptr)
		{
			throw TextureImageUpdateException("Cannot update an empty TextureImage");
		}
		void* pixelptr;
		int pitch;
		if(SDL_LockTexture((SDL_Texture*)texture, nullptr, &pixelptr, &pitch) < 0)
		{
			throw TextureImageUpdateException(SDL_GetError());
		}
		
		fgl::Uint32*texture_pixels = (fgl::Uint32*)pixelptr;
		unsigned int total = width*height;
		for(unsigned int i=0; i<total; i++)
		{
			const Color& color = pxls[i];
			if(color.a > 0)
			{
				pixels[i]=true;
			}
			else
			{
				pixels[i] = false;
			}
			texture_pixels[i] = color.getRGBA();
		}
		
		SDL_UnlockTexture((SDL_Texture*)texture);
		SDL_SetTextureBlendMode((SDL_Texture*)texture,SDL_BLENDMODE_BLEND);
	}

	//TODO figure out where the Rect updates first, so I can figure out how to update the bool vector
	//TODO add checking for out of bounds
	/*void TextureImage::update(const Color*pixels, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		if(texture == nullptr)
		{
			throw TextureImageUpdateException("Cannot update an empty TextureImage");
		}

		SDL_Rect rect;
		rect.x = (int)x;
		rect.y = (int)y;
		rect.w = (int)w;
		rect.h = (int)h;

		void* pixelptr;
		int pitch;
		if(SDL_LockTexture((SDL_Texture*)texture, &rect, &pixelptr, &pitch) < 0)
		{
			throw TextureImageUpdateException(SDL_GetError());
		}

		Color*texture_pixels = (Color*)pixelptr;
		unsigned int total = w*h;
		for(unsigned int i=0; i<total; i++)
		{
			texture_pixels[i] = pixels[i];
		}

		SDL_UnlockTexture((SDL_Texture*)texture);
	}*/
	
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

	SDL_Texture* TextureImage_loadFromSDLSurface(SDL_Surface* surface, std::vector<bool>& pixels, SDL_Renderer* renderer, String* error)
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
				return nullptr;
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

		unsigned int w = (unsigned int)surface->w;
		unsigned int h = (unsigned int)surface->h;
		unsigned int total = w*h;

		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, (int)w, (int)h);
		if(texture == nullptr)
		{
			if(error!=nullptr)
			{
				*error = SDL_GetError();
			}
			SDL_UnlockSurface(surface);
			return nullptr;
		}

		void*pixelptr;
		int pitch;
		if(SDL_LockTexture(texture, nullptr, &pixelptr, &pitch) < 0)
		{
			if(error!=nullptr)
			{
				*error = SDL_GetError();
			}
			SDL_DestroyTexture(texture);
			SDL_UnlockSurface(surface);
			return nullptr;
		}

		pixels.resize(total);
		pixels.shrink_to_fit();

		unsigned int pitchDif = ((unsigned int)surface->pitch - (w*bpp));

		unsigned int counter = 0;
		byte*surfacePixels = (byte*)surface->pixels;
		int*texture_pixels = (int*)pixelptr;

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
				unsigned int abgr = px.getRGBA();
				texture_pixels[i] = abgr;
				i++;
				counter += bpp;
			}
			counter += pitchDif;
		}

		SDL_UnlockTexture(texture);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		if(mustlock != 0)
		{
			SDL_UnlockSurface(surface);
		}

		return texture;
	}

	bool TextureImage::loadFromPointer(const void* pointer, size_t size, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(pointer, size), 1);
		if(surface != nullptr)
		{
			SDL_Texture* newTexture = TextureImage_loadFromSDLSurface(surface, pixels, (SDL_Renderer*)graphics.renderer, error);
			if(newTexture!=nullptr)
			{
				width = (unsigned int)surface->w;
				height = (unsigned int)surface->h;
				SDL_FreeSurface(surface);
				if(texture != nullptr)
				{
					SDL_DestroyTexture((SDL_Texture*)texture);
					texture = nullptr;
				}
				texture = newTexture;
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
	
	bool TextureImage::loadFromFile(const String& path, Graphics& graphics, String* error)
	{
		SDL_Surface* surface = IMG_Load(path);
		if(surface != nullptr)
		{
			SDL_Texture* newTexture = TextureImage_loadFromSDLSurface(surface, pixels, (SDL_Renderer*)graphics.renderer, error);
			if(newTexture!=nullptr)
			{
				width = (unsigned int)surface->w;
				height = (unsigned int)surface->h;
				SDL_FreeSurface(surface);
				if(texture != nullptr)
				{
					SDL_DestroyTexture((SDL_Texture*)texture);
					texture = nullptr;
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