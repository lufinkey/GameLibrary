
#pragma once

#include <mutex>
#include <GameLibrary/Types.h>
#include <GameLibrary/Utilities/ArrayList.h>
#include <GameLibrary/Utilities/Dictionary.h>
#include <GameLibrary/Utilities/String.h>

namespace fgl
{
	/*! Stores rendered font glyphs.*/
	class RenderedGlyphContainer
	{
		friend class Font;
		friend class Graphics;
	public:
		typedef fgl::Uint16 glyph_char;
		typedef BasicString<glyph_char> GlyphString;
		
		/*! information for a rendered font glyph*/
		typedef struct
		{
			void* texture;
			unsigned int size;
			int fontstyle;
			bool antialias;
		} RenderedGlyph;
		
		/*! default constructor*/
		RenderedGlyphContainer();
		/*! destructor*/
		~RenderedGlyphContainer();
		
		/*! Gets an array of rendered font glyphs for a given string.
			\param fontptr an SDL font pointer (a TTF_Font pointer)
			\param renderer an SDL_Renderer pointer
			\param size a desired glyph size
			\param fontstyle the desired font style \see fgl::Font::Style
			\param txt the string of text to render glyphs for
			\param antialiasing tell whether smoothing is enabled
			\returns an ArrayList of RendererdGlyph structures*/
		ArrayList<RenderedGlyph> getRenderedGlyphs(void*fontptr, void*renderer, unsigned int size, int fontstyle, const GlyphString&txt, bool antialiasing = false);
		
		/*! Removes and deletes all stored glyph renders.*/
		void clear();
		
	private:
		typedef struct
		{
			ArrayList<RenderedGlyph> styles;
		} RenderedGlyphStyles;
		
		ArrayList<Pair<glyph_char, RenderedGlyphStyles*> > glyphs;
		std::mutex mlock;
		
		RenderedGlyph renderGlyph(glyph_char glyph, void*fontptr, void*renderer, unsigned int size, int fontstyle, bool antialiasing);
		void destroyGlyph(RenderedGlyph&renderedGlyph);
		void destroyGlyphStyles(RenderedGlyphStyles*glyphStyles);
		
		RenderedGlyph getGlyph(glyph_char glyph, void*fontptr, void*renderer, unsigned int size, int fontstyle, bool antialiasing);
		RenderedGlyph findGlyph(const ArrayList<RenderedGlyph>&glyphStyles, unsigned int size, int fontstyle, bool antialiasing);
	};
}