
#include <GameLibrary/Exception/NotImplementedException.h>
#include "ExceptionMacros.h"

namespace GameLibrary
{
	NotImplementedException::NotImplementedException(const String& message) : Exception(message)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(NotImplementedException)
}
