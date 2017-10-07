
#include <GameLibrary/Exception/Graphics/TextureImageUpdateException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	TextureImageUpdateException::TextureImageUpdateException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(TextureImageUpdateException)
}
