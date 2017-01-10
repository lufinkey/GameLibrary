
#pragma once

#include "Exception.hpp"

namespace fgl
{
	/*! Thrown to indicate that a function has been invoked at an illegal or inappropriate time*/
	class IllegalStateException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		explicit IllegalStateException(const String& message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
