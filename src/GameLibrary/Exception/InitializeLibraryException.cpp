
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include "ExceptionMacros.hpp"

namespace fgl
{
	InitializeLibraryException::InitializeLibraryException(const String&lib, const String&message)
		: Exception(lib + ": " + message),
		library(lib)
	{
		//
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(InitializeLibraryException)
}
