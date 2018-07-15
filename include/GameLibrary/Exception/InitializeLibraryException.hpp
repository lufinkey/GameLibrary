
#pragma once

#include "Exception.hpp"

namespace fgl
{
	/*! Thrown to indicate that an error occurred while initializing a base Application library*/
	class InitializeLibraryException : public Exception
	{
	public:
		/*! The name of the library that threw an error*/
		String library;
		
		/*! Constructs an exception with a library name and an error message.
			\param library the name of the library that threw an error
			\param message an explanation of the error*/
		InitializeLibraryException(const String& library, const String& message);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl::InitializeLibraryException, fgl::Exception)
