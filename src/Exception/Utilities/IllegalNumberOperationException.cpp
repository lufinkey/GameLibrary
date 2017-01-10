
#include <GameLibrary/Exception/Utilities/IllegalNumberOperationException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
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
