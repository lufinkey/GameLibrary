
#pragma once

#include <GameLibrary/Exception/OutOfBoundsException.h>

namespace GameLibrary
{
	/*! Thrown to indicate that an index to a DataPacket was out of bounds*/
	class DataOutOfBoundsException : public OutOfBoundsException
	{
	public:
		/*! The out of bounds index*/
		size_t index;
		/*! The size of the DataPacket*/
		size_t size;
		
		/*! Constructs an exception with a DataPacket index and size.
			\param index the index in the DataPacket
			\param size the size of the DataPacket*/
		DataOutOfBoundsException(size_t index, size_t size);
		
		/*! \copydoc GameLibrary::Exception::getClassName()const*/
		virtual String getClassName() const override;
	};
}
