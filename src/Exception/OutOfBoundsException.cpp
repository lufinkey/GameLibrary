
#include <GameLibrary/Exception/OutOfBoundsException.h>
#include "ExceptionMacros.h"

namespace fgl
{
	OutOfBoundsException::OutOfBoundsException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(OutOfBoundsException)
}
