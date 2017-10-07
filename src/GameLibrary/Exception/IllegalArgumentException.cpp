
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include "ExceptionMacros.hpp"

namespace fgl
{
	IllegalArgumentException::IllegalArgumentException(const String&arg, const String&reason)
		: Exception("argument " + arg + " is not a valid argument: " + reason),
		argument(arg)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(IllegalArgumentException)
}
