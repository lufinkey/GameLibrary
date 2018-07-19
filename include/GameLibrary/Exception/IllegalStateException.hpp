
#pragma once

#include "Exception.hpp"

namespace fgl
{
	/*! Thrown to indicate that a function has been invoked at an illegal or inappropriate time*/
	class IllegalStateException : public Exception
	{
	public:
		using Exception::Exception;
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
