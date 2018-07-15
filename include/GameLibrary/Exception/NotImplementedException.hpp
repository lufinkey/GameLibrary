
#pragma once

#include "Exception.hpp"

namespace fgl
{
	/*! Thrown to indicate that some feature cannot be used since it is not yet implemented*/
	class NotImplementedException : public Exception
	{
	public:
		/*! \copydoc fgl::Exception::Exception(const fgl::String&)*/
		NotImplementedException(const String& message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl::NotImplementedException, fgl::Exception)
