
#pragma once

#include "Exception.h"

namespace GameLibrary
{
	/*! Thrown to indicate that an index of some sort (such as to an array, to a string, or to a vector) is out of bounds*/
	class OutOfBoundsException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		explicit OutOfBoundsException(const String&message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}