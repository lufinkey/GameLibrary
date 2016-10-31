
#include <GameLibrary/Exception/Exception.h>
#include "ExceptionMacros.h"

namespace fgl
{
	Exception::Exception()
	{
		//
	}
	
	Exception::Exception(const String& msg)
#ifdef _MSC_VER
		: std::exception(msg),
		message(msg)
#else
		: message(msg)
#endif
	{
		//
	}
	
	Exception::~Exception()
	{
		//
	}
	
	const char* Exception::what() const noexcept
	{
		return message;
	}
	
	String Exception::toString() const
	{
		return what();
	}
	
	GAMELIB_MEMBERFUNC_GETCLASSNAME(Exception)
}