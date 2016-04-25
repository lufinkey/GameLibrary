
#pragma once

#include "Exception.h"

namespace GameLibrary
{
	/*! Thrown to indicate that a function has been passed an illegal or inappropriate argument*/
	class IllegalArgumentException : public Exception
	{
	public:
		/*! The parameter name of the argument that was "illegal"*/
		String argument;
		
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		IllegalArgumentException(const String&argument, const String&reason);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}