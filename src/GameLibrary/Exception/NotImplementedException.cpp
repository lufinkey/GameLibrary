
#include <GameLibrary/Exception/NotImplementedException.hpp>
#include "ExceptionMacros.hpp"

namespace fgl
{
	NotImplementedException::NotImplementedException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(NotImplementedException)
}
