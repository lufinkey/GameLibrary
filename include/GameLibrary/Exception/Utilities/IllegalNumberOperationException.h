
#pragma once

#include <GameLibrary/Exception/Exception.h>

namespace GameLibrary
{
	/*! Thrown to indicate that an illegal operator was used on a Number object.
		\see GameLibrary::Number*/
	class IllegalNumberOperationException : public Exception
	{
	public:
		String operatorType;
		String reason;
		
		/*! Constructs an exception with an operator, a number type, and the equation side.
			\param operatorType the type of operator used on the Number object (+, -, +=, etc.)
			\param numberType the type of number primitive used (int, bool, float, double, etc.)
			\param reason the actual reason for the exception*/
		IllegalNumberOperationException(const String& operatorType, const String& reason);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
