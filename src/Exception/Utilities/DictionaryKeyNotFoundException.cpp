
#include <GameLibrary/Exception/Utilities/DictionaryKeyNotFoundException.h>
#include "../ExceptionMacros.h"

namespace fgl
{
	DictionaryKeyNotFoundException::DictionaryKeyNotFoundException(const String& keyString)
		: OutOfBoundsException("key \""+keyString+"\" not found in BasicDictionary object"),
		keyString(keyString)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(DictionaryKeyNotFoundException)
}
