
#include <GameLibrary/Exception/Graphics/TextureImageCreateException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	TextureImageCreateException::TextureImageCreateException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(TextureImageCreateException)
}
