
#pragma once

#include "Exception.hpp"

namespace fgl
{
	/*! Thrown to indicate a function has returned an illegal or inappropriate value */
	class IllegalReturnValueException : public Exception
	{
	public:
		using Exception::Exception;
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, IllegalReturnValueException, fgl::Exception)
