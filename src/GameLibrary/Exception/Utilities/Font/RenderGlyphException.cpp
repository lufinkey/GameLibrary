
#include <GameLibrary/Exception/Utilities/Font/RenderGlyphException.hpp>
#include "../../ExceptionMacros.hpp"

namespace fgl
{
	RenderGlyphException::RenderGlyphException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(RenderGlyphException)
}
