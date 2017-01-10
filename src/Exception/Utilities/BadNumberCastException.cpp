
#include <GameLibrary/Exception/Utilities/BadNumberCastException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	BadNumberCastException::BadNumberCastException(const String& casttype) : BadCastException("Number", casttype)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadNumberCastException)
}
