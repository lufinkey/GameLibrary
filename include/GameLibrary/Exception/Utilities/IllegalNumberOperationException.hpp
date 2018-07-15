
#pragma once

#include <GameLibrary/Exception/Exception.hpp>

namespace fgl
{
	/*! Thrown to indicate that an illegal operator was used on a Number object.
		\see fgl::Number*/
	class IllegalNumberOperationException : public Exception
	{
	public:
		String operatorType;
		String reason;
		
		/*! Constructs an exception with an operator, a number type, and the equation side.
			\param operatorType the type of operator used on the Number object (+, -, +=, etc.)
			\param reason the actual reason for the exception*/
		IllegalNumberOperationException(const String& operatorType, const String& reason);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, IllegalNumberOperationException, fgl::Exception)
