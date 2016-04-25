
#include <GameLibrary/Exception/IllegalArgumentException.h>
#include "ExceptionMacros.h"

namespace GameLibrary
{
	IllegalArgumentException::IllegalArgumentException(const String&arg, const String&reason)
		: Exception("argument " + arg + " is not a valid argument: " + reason),
		argument(arg)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(IllegalArgumentException)
}