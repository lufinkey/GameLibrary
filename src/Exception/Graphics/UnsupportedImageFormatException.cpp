
#include <GameLibrary/Exception/Graphics/UnsupportedImageFormatException.hpp>
#include "../ExceptionMacros.hpp"

namespace fgl
{
	UnsupportedImageFormatException::UnsupportedImageFormatException(const String&formatType)
		: Exception((formatType.length() == 0 ? "Unspecified Image format" : ((String)"Unsupported Image format \"" + formatType + "\""))),
		format(formatType)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(UnsupportedImageFormatException)
}
