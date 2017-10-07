
#include <GameLibrary/Exception/OutOfBoundsException.hpp>
#include "ExceptionMacros.hpp"

namespace fgl
{
	OutOfBoundsException::OutOfBoundsException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(OutOfBoundsException)
}
