
#include <GameLibrary/Exception/Utilities/DataOutOfBoundsException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	DataOutOfBoundsException::DataOutOfBoundsException(size_t index, size_t size)
		: OutOfBoundsException((String)"index " + index + " is out of bounds in Data with size of " + size),
		index(index),
		size(size)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(DataOutOfBoundsException)
}
