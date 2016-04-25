
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace GameLibrary
{
	/*! Thrown to indicate an error rendering a font glyph*/
	class RenderGlyphException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		explicit RenderGlyphException(const String&message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
