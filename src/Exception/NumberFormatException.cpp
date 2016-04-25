
#include <GameLibrary/Exception/NumberFormatException.h>
#include "ExceptionMacros.h"

namespace GameLibrary
{
	NumberFormatException::NumberFormatException(const String& type, const String& reason)
		: Exception((String)"Unable to convert String to type " + type + ": " + reason),
		type(type),
		reason(reason)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(NumberFormatException)
}
