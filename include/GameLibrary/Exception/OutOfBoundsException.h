
#pragma once

#include "Exception.h"

namespace fgl
{
	/*! Thrown to indicate that an index of some sort (such as to an array, to a string, or to a vector) is out of bounds*/
	class OutOfBoundsException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit OutOfBoundsException(const String&message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}