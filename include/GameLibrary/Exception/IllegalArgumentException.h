
#pragma once

#include "Exception.h"

namespace fgl
{
	/*! Thrown to indicate that a function has been passed an illegal or inappropriate argument*/
	class IllegalArgumentException : public Exception
	{
	public:
		/*! The parameter name of the argument that was "illegal"*/
		String argument;
		
		/*! Constructs an exception with the name of the illegal argument and the reason it was illegal
			\param argument the name of the illegal parameter
			\param reason the reason the parameter was illegal*/
		IllegalArgumentException(const String& argument, const String& reason);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}