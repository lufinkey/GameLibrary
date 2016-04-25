
#include <GameLibrary/Exception/Graphics/TextureImageUpdateException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	TextureImageUpdateException::TextureImageUpdateException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(TextureImageUpdateException)
}
