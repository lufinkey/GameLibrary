
#pragma once

#include <GameLibrary/Exception/Exception.hpp>

namespace fgl
{
	/*! Thrown to indicate an error rendering a font glyph*/
	class RenderGlyphException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit RenderGlyphException(const String&message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
