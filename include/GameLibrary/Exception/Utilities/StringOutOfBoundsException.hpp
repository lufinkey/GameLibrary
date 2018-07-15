
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.hpp>

namespace fgl
{
	/*! Thrown to indicate that an index to a String was out of bounds*/
	class StringOutOfBoundsException : public OutOfBoundsException
	{
	public:
		/*! The index in the string*/
		size_t index;
		/*! The length of the string*/
		size_t length;
		
		/*! Constructs an exception with a String index and length.
			\param index the index in the String
			\param length the length of the String*/
		StringOutOfBoundsException(size_t index, size_t length);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, StringOutOfBoundsException, fgl::OutOfBoundsException)
