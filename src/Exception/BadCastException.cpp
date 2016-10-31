
#include <GameLibrary/Exception/BadCastException.h>
#include "ExceptionMacros.h"

namespace fgl
{
	BadCastException::BadCastException(const String& origType, const String& castType)
		: Exception((String)"Bad cast from type " + origType + " to type " + castType),
		originalType(origType),
		castedType(castType)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(BadCastException)
}