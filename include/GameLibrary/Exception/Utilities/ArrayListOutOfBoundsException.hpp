
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.hpp>

namespace fgl
{
	/*! Thrown to indicate that an index to an ArrayList was out of bounds*/
	class ArrayListOutOfBoundsException : public OutOfBoundsException
	{
	public:
		/*! the out of bounds index*/
		size_t index;
		/*! the size of the ArrayList*/
		size_t size;
		
		/*! Constructs an exception with an ArrayList index and size
			\param index the index in the ArrayList
			\param size the size of the ArrayList*/
		ArrayListOutOfBoundsException(size_t index, size_t size);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, ArrayListOutOfBoundsException, fgl::OutOfBoundsException)
