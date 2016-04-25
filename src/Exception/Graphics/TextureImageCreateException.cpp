
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	TextureImageCreateException::TextureImageCreateException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(TextureImageCreateException)
}
