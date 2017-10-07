
#include <GameLibrary/Exception/Utilities/DictionaryKeyNotFoundException.hpp>
#include "../ExceptionMacros.hpp"

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
