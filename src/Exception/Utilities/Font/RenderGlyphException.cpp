
#include <GameLibrary/Exception/Utilities/Font/RenderGlyphException.h>
#include "../../ExceptionMacros.h"

namespace fgl
{
	RenderGlyphException::RenderGlyphException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(RenderGlyphException)
}
