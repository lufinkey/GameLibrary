
#include <GameLibrary/Exception/Utilities/BadNumberCastException.h>
#include "../ExceptionMacros.h"

namespace fgl
{
	BadNumberCastException::BadNumberCastException(const String& casttype) : BadCastException("Number", casttype)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadNumberCastException)
}
