
#include <GameLibrary/Exception/Utilities/ArrayListOutOfBoundsException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	ArrayListOutOfBoundsException::ArrayListOutOfBoundsException(size_t index, size_t size)
		: OutOfBoundsException((String)"index " + index + " is out of bounds in ArrayList with size of " + size),
		index(index),
		size(size)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(ArrayListOutOfBoundsException)
}
