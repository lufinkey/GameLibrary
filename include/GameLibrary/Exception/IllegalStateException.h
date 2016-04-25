
#pragma once

#include "Exception.h"

namespace GameLibrary
{
	/*! Thrown to indicate that a function has been invoked at an illegal or inappropriate time*/
	class IllegalStateException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		explicit IllegalStateException(const String&message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
