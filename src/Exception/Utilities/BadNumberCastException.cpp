
#include <GameLibrary/Exception/Utilities/BadNumberCastException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	BadNumberCastException::BadNumberCastException(const String& casttype) : BadCastException("Number", casttype)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadNumberCastException)
}
