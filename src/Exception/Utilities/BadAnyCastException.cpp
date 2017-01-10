
#include <GameLibrary/Exception/Utilities/BadAnyCastException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	BadAnyCastException::BadAnyCastException(const String& casttype) : BadCastException("Any", casttype)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadAnyCastException)
}
