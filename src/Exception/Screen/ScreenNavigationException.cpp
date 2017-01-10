
#include <GameLibrary/Exception/Screen/ScreenNavigationException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	ScreenNavigationException::ScreenNavigationException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(ScreenNavigationException)
}
