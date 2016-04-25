
#include <GameLibrary/Exception/Graphics/UnsupportedImageFormatException.h>
#include "../ExceptionMacros.h"

namespace GameLibrary
{
	UnsupportedImageFormatException::UnsupportedImageFormatException(const String&formatType)
		: Exception((formatType.length() == 0 ? "Unspecified Image format" : ((String)"Unsupported Image format \"" + formatType + "\""))),
		format(formatType)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(UnsupportedImageFormatException)
}