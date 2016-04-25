
#include <GameLibrary/Exception/Utilities/IllegalNumberOperationException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	IllegalNumberOperationException::IllegalNumberOperationException(const String& operatorType, const String& reason)
		: Exception("Error perform operation \"" + operatorType + "\": "+reason),
		operatorType(operatorType),
		reason(reason)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(IllegalNumberOperationException)
}