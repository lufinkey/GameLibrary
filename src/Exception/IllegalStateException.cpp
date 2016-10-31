
#include <GameLibrary/Exception/IllegalStateException.h>
#include "ExceptionMacros.h"

namespace fgl
{
	IllegalStateException::IllegalStateException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(IllegalStateException)
}
