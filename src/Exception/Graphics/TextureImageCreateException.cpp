
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.h>
#include "../ExceptionMacros.h"

namespace fgl
{
	TextureImageCreateException::TextureImageCreateException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(TextureImageCreateException)
}
