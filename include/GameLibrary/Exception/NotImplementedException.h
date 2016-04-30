
#pragma once

#include "Exception.h"

namespace GameLibrary
{
	/*! Thrown to indicate that some feature cannot be used since it is not yet implemented*/
	class NotImplementedException : public Exception
	{
	public:
		/*! \copydoc GameLibrary::Exception::Exception(const GameLibrary::String&)*/
		NotImplementedException(const String& message);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
