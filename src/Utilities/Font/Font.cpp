
#include <GameLibrary/Utilities/Font/Font.hpp>
#include <GameLibrary/Utilities/Data.hpp>
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include <GameLibrary/Exception/Utilities/Font/RenderGlyphException.hpp>
#include <SDL_ttf.h>
#include <memory>

namespace fgl
{
	int Font::styleToTTFStyle(int fontstyle)
	{
		if(fontstyle == Font::STYLE_PLAIN)
		{
			return TTF_STYLE_NORMAL;
		}
		else
		{
			int stylemask = 0;
			if((fontstyle & Font::STYLE_BOLD) == Font::STYLE_BOLD)
			{
				stylemask = stylemask | TTF_STYLE_BOLD;
			}
			if((fontstyle & Font::STYLE_ITALIC) == Font::STYLE_ITALIC)
			{
				stylemask = stylemask | TTF_STYLE_ITALIC;
			}
			if((fontstyle & Font::STYLE_UNDERLINE) == Font::STYLE_UNDERLINE)
			{
				stylemask = stylemask | TTF_STYLE_UNDERLINE;
			}
			if((fontstyle & Font::STYLE_STRIKETHROUGH) == Font::STYLE_STRIKETHROUGH)
			{
				stylemask = stylemask | TTF_STYLE_STRIKETHROUGH;
			}
			return stylemask;
		}
	}

	void* Font::loadFontSize(unsigned int size)
	{
		Data& fontDataPacket = *fontData.get();
		SDL_RWops* ops = SDL_RWFromConstMem(fontDataPacket.getData(), (int)fontDataPacket.size());
		if(ops == nullptr)
		{
			return nullptr;
		}
		TTF_Font* loadedfont = TTF_OpenFontRW(ops,0,size);
		if(loadedfont == nullptr)
		{
			return nullptr;
		}
		return (void*)loadedfont;
	}

	void* Font::getFontPtr(unsigned int size)
	{
		FontSizeList& sizeList = *fontSizes.get();
		size_t total = sizeList.size();
		for(size_t i=0; i<total; i++)
		{
			const std::pair<unsigned int, void*>& fontSize = sizeList.get(i);
			if(fontSize.first == size)
			{
				return fontSize.second;
			}
		}

		void* fontsizeptr = loadFontSize(size);
		if(fontsizeptr != nullptr)
		{
			sizeList.add(std::pair<unsigned int, void*>(size, fontsizeptr));
		}
		return fontsizeptr;
	}

	int Font::getAscent(unsigned int size)
	{
		void* fontptr = getFontPtr(size);
		return TTF_FontAscent((TTF_Font*)fontptr);
	}

	int Font::getDescent(unsigned int size)
	{
		void* fontptr = getFontPtr(size);
		return TTF_FontDescent((TTF_Font*)fontptr);
	}

	void Font::clearFontSizes()
	{
		if(fontSizes.get()!=nullptr)
		{
			FontSizeList& sizeList = *fontSizes.get();
			for(unsigned int i=0; i<sizeList.size(); i++)
			{
				std::pair<unsigned int, void*>& fontSize = sizeList.get(i);
				TTF_Font* fontSizeData = (TTF_Font*)fontSize.second;
				TTF_CloseFont(fontSizeData);
				fontSize.second = nullptr;
			}
			sizeList.clear();
		}
	}

	Font::Font()
	{
		if(!TTF_WasInit())
		{
			if(TTF_Init() < 0)
			{
				throw InitializeLibraryException("SDL_ttf", TTF_GetError());
			}
		}

		size = 24;
		style = STYLE_PLAIN;
		fontData = nullptr;
		antialiasing = true;
	}

	Font::Font(const Font&font)
	{
		font.mlock.lock();
		size = font.size;
		style = font.style;
		antialiasing = font.antialiasing;
		fontData = font.fontData;
		fontSizes = font.fontSizes;
		font.mlock.unlock();
	}

	Font::~Font()
	{
		clearFontSizes();
		for(unsigned int i=0; i<glyphs.size(); i++)
		{
			delete glyphs.get(i).first;
		}
		glyphs.clear();
	}
	
	Font& Font::operator=(const Font&font)
	{
		mlock.lock();
		
		clearFontSizes();
		for(unsigned int i=0; i<glyphs.size(); i++)
		{
			delete glyphs.get(i).first;
		}
		glyphs.clear();

		font.mlock.lock();
		size = font.size;
		style = font.style;
		antialiasing = font.antialiasing;
		fontData = font.fontData;
		fontSizes = font.fontSizes;
		font.mlock.unlock();
		mlock.unlock();
		
		return *this;
	}

	bool Font::loadFromPath(const String& path, String*error)
	{
		FILE* file = std::fopen(path, "r");
		if (file == nullptr)
		{
			//TODO add switch for errno
			if(error!=nullptr)
			{
				*error = "Unable to load data from file";
			}
			return false;
		}
		bool success = loadFromFile(file, error);
		std::fclose(file);
		return success;
	}
	
	bool Font::loadFromFile(FILE* file, String* error)
	{
		if(file==nullptr)
		{
			throw fgl::IllegalArgumentException("file", "cannot be null");
		}
		mlock.lock();
		Data* fontDataPacket = new Data();
		if(!fontDataPacket->loadFromFile(file, error))
		{
			delete fontDataPacket;
			mlock.unlock();
			return false;
		}
		
		SDL_RWops* ops = SDL_RWFromConstMem(fontDataPacket->getData(), (int)fontDataPacket->size());
		if(ops == nullptr)
		{
			if(error!=nullptr)
			{
				*error = SDL_GetError();
			}
			delete fontDataPacket;
			mlock.unlock();
			return false;
		}
		
		TTF_Font* loadedfont = TTF_OpenFontRW(ops,0,(int)size);
		if(loadedfont == nullptr)
		{
			if(error!=nullptr)
			{
				*error = TTF_GetError();
			}
			if(SDL_RWclose(ops) != 0)
			{
				if(error!=nullptr)
				{
					*error = (String)"" + *error + "\n" + SDL_GetError();
				}
			}
			delete fontDataPacket;
			mlock.unlock();
			return false;
		}
		glyphs.clear();
		fontData = std::shared_ptr<Data>(fontDataPacket);
		if(fontSizes != nullptr)
		{
			clearFontSizes();
			fontSizes = nullptr;
		}
		fontSizes = std::shared_ptr<FontSizeList>(new FontSizeList());
		fontSizes->add(std::pair<unsigned int, void*>(size, (void*)loadedfont));
		mlock.unlock();
		return true;
	}

	RenderedGlyphContainer* Font::getRenderedGlyphContainer(void*renderer)
	{
		for(unsigned int i=0; i<glyphs.size(); i++)
		{
			std::pair<RenderedGlyphContainer*, void*>& container = glyphs.get(i);
			if(container.second == renderer)
			{
				return container.first;
			}
		}
		std::pair<RenderedGlyphContainer*, void*> container;
		container.first = new RenderedGlyphContainer();
		container.second = renderer;
		glyphs.add(container);
		return container.first;
	}

	ArrayList<RenderedGlyphContainer::RenderedGlyph> Font::getRenderedGlyphs(const GlyphString& text, void* renderer, unsigned int size, int style)
	{
		if(fontData!=nullptr)
		{
			mlock.lock();
			RenderedGlyphContainer* container = getRenderedGlyphContainer(renderer);
			ArrayList<RenderedGlyphContainer::RenderedGlyph> renderedGlyphs;
			try
			{
				renderedGlyphs = container->getRenderedGlyphs(getFontPtr(size),renderer,size,style,text,antialiasing);
			}
			catch(const RenderGlyphException& e)
			{
				mlock.unlock();
				throw;
			}
			mlock.unlock();
			return renderedGlyphs;
		}
		return ArrayList<RenderedGlyphContainer::RenderedGlyph>();
	}
	
	Vector2u Font::measureString(const String& text)
	{
		return measureString((GlyphString)text);
	}
	
	Vector2u Font::measureString(const WideString& text)
	{
		return measureString((GlyphString)text);
	}

	Vector2u Font::measureString(const GlyphString& text)
	{
		mlock.lock();
		if(fontData == nullptr || fontSizes == nullptr)
		{
			mlock.unlock();
			return Vector2u(0,0);
		}
		TTF_Font* font = (TTF_Font*)getFontPtr(size);
		TTF_SetFontStyle(font, styleToTTFStyle(style));
		int descent = TTF_FontDescent(font);
		int totalWidth = 0;
		int totalHeight = 0;
		for(unsigned int i=0; i<text.length(); i++)
		{
			int w = 0;
			int h = 0;
			glyph_char str[2];
			str[0] = text.charAt(i);
			str[1] = NULL;
			if(TTF_SizeUNICODE(font, str, &w, &h) < 0)
			{
				mlock.unlock();
				//TODO replace with more specific exception type
				throw Exception(TTF_GetError());
			}
			if(h > totalHeight)
			{
				totalHeight = h;
			}
			totalWidth += w;
		}
		int fixedHeight = totalHeight - descent;
		if(fixedHeight < 0)
		{
			fixedHeight = 0;
		}
		mlock.unlock();
		return Vector2u((unsigned int)totalWidth, (unsigned int)fixedHeight);
	}

	void Font::setStyle(int s)
	{
		mlock.lock();
		style = s;
		mlock.unlock();
	}

	void Font::setSize(unsigned int s)
	{
		mlock.lock();
		size = s;
		mlock.unlock();
	}

	void Font::setAntialiasing(bool toggle)
	{
		mlock.lock();
		antialiasing = toggle;
		mlock.unlock();
	}

	int Font::getStyle()
	{
		return style;
	}

	unsigned int Font::getSize()
	{
		return size;
	}

	bool Font::getAntialiasing()
	{
		return antialiasing;
	}
}
