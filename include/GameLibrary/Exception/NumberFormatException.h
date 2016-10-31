
#pragma once

#include "Exception.h"

namespace fgl
{
	/*! Thrown to indicate that an attempt was made to convert a String to a numeric type, but that the String does not have the appropriate format*/
	class NumberFormatException : public Exception
	{
	public:
		/*! The numeric type the String was being converted to*/
		String type;
		/*! The reason the conversion failed*/
		String reason;
		
		/*! Constructs an exception with a numeric type and an error message.
			\param type the numeric type that a String was being converted to
			\param reason the reason that the numeric conversion failed*/
		NumberFormatException(const String& type, const String& reason);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}