
#include <GameLibrary/Exception/IllegalStateException.hpp>
#include "ExceptionMacros.hpp"

namespace fgl
{
	IllegalStateException::IllegalStateException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(IllegalStateException)
}
