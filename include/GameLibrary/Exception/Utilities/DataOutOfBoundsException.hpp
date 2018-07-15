
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.hpp>

namespace fgl
{
	/*! Thrown to indicate that an index to a Data object was out of bounds*/
	class DataOutOfBoundsException : public OutOfBoundsException
	{
	public:
		/*! The out of bounds index*/
		size_t index;
		/*! The size of the DataPacket*/
		size_t size;
		
		/*! Constructs an exception with a Data index and size.
			\param index the index in the Data object
			\param size the size of the Data object*/
		DataOutOfBoundsException(size_t index, size_t size);
		
		/*! \copydoc fgl::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}

REGISTER_TYPE(fgl, DataOutOfBoundsException, fgl::OutOfBoundsException)
