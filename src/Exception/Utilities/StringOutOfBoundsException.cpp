
#include <GameLibrary/Exception/Utilities/StringOutOfBoundsException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	StringOutOfBoundsException::StringOutOfBoundsException(size_t index, size_t length)
		: OutOfBoundsException((String)"index " + index + " is out of bounds in String with length of " + length)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(StringOutOfBoundsException)
}