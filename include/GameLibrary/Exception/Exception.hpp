
#pragma once

#include <exception>
#include <GameLibrary/Utilities/BasicString.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	/*! An exception class to throw when a generic error arises*/
	class Exception : std::exception
	{
	public:
		/*! An explanation of the error*/
		String message;
		
		/*! default constructor*/
		Exception();
		/*! Constructs an exception with an error message.
			\param message an explanation of the error*/
		explicit Exception(const String& message);
		/*! virtual destructor*/
		virtual ~Exception();
		
		/*! Gets an explanation of the error.
			\returns a const char pointer containing an error message*/
		virtual const char* what() const noexcept;
		
		/*! Gets a String representation of the Exception
			\returns a String representation of the exception*/
		String toString() const;
		
		/*! Gets the name of the Exception class. This function must be overridden in every subclass of Exception.
			\returns a String representing the class name of the exception*/
		virtual String getClassName() const;
	};
}

REGISTER_TYPE(fgl, Exception)
