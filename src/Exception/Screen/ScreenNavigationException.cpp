
#include <GameLibrary/Exception/Screen/ScreenNavigationException.h>
#include "../ExceptionMacros.h"

namespace fgl
{
	ScreenNavigationException::ScreenNavigationException(const String&message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(ScreenNavigationException)
}
