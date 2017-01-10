
#include <GameLibrary/Exception/Utilities/StringOutOfBoundsException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	StringOutOfBoundsException::StringOutOfBoundsException(size_t index, size_t length)
		: OutOfBoundsException((String)"index " + index + " is out of bounds in String with length of " + length)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(StringOutOfBoundsException)
}
