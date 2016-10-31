
#include <GameLibrary/Exception/Utilities/BadAnyCastException.h>
#include "../ExceptionMacros.h"

namespace fgl
{
	BadAnyCastException::BadAnyCastException(const String& casttype) : BadCastException("Any", casttype)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadAnyCastException)
}