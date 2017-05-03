
#define _CRT_SECURE_NO_WARNINGS

#include <GameLibrary/Utilities/Data.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/Utilities/DataOutOfBoundsException.hpp>
#include <cstdio>
#include <cstdlib>

namespace fgl
{
	Data::Data()
	{
		data = nullptr;
		length = 0;
	}

	Data::Data(const Data& dataPacket)
	{
		if(dataPacket.data == nullptr)
		{
			data = nullptr;
			length = 0;
		}
		else
		{
			data = (byte*)std::malloc(dataPacket.length);
			for(size_t i=0; i<dataPacket.length; i++)
			{
				data[i] = dataPacket.data[i];
			}
			length = dataPacket.length;
		}
	}
	
	Data::Data(Data&& dataPacket)
		: data(dataPacket.data),
		length(dataPacket.length)
	{
		dataPacket.data = nullptr;
		dataPacket.length = 0;
	}
	
	Data::Data(const String& str)
	{
		if(str.length()==0)
		{
			data = nullptr;
			length = 0;
		}
		{
			data = (byte*)std::malloc(str.length());
			const byte* cpyBytes = (const byte*)((const char*)str);
			for(size_t i = 0; i < str.length(); i++)
			{
				data[i] = cpyBytes[i];
			}
			length = str.length();
		}
	}
	
	Data::Data(size_t size)
	{
		data = (byte*)std::malloc(size);
		for(size_t i=0; i<size; i++)
		{
			data[i] = 0;
		}
		length = size;
	}
	
	Data::Data(const void*data_arg, size_t size)
	{
		if(data_arg == nullptr)
		{
			throw IllegalArgumentException("data", "null");
		}
		else if(size>0)
		{
			data = (byte*)std::malloc(size);
			const byte*copyBytes = (const byte*)data_arg;
			for(size_t i=0; i<size; i++)
			{
				data[i] = copyBytes[i];
			}
			length = size;
		}
		else
		{
			data = nullptr;
			length = 0;
		}
	}
	
	Data::~Data()
	{
		if(data!=nullptr)
		{
			std::free(data);
			data = nullptr;
			length = 0;
		}
	}
	
	Data& Data::operator=(const Data& dataPacket)
	{
		assign(dataPacket);
		return *this;
	}
	
	bool Data::loadFromPath(const String& path, String* error)
	{
		FILE*file = std::fopen(path, "r");
		if (file == nullptr)
		{
			//TODO add switch for errno
			if(error!=nullptr)
			{
				*error = "Unable to load data from file";
			}
			return false;
		}
		std::fseek(file, 0, SEEK_END);
		size_t fileSize = (size_t)std::ftell(file);
		std::fseek(file, 0, SEEK_SET);
		if(data == nullptr)
		{
			data = (byte*)std::malloc(fileSize);
		}
		else
		{
			data = (byte*)std::realloc(data, fileSize);
		}
		std::fread((void*)data, 1, fileSize, file);
		std::fclose(file);
		length = (size_t)fileSize;
		return true;
	}
	
	bool Data::saveToPath(const String& path, String* error) const
	{
		FILE*file = std::fopen(path, "w");
		if(file==NULL)
		{
			if(error != nullptr)
			{
				//TODO add checking of errno
				*error = "Unable to open file for writing";
			}
			return false;
		}
		
		size_t written = std::fwrite(data, 1, length, file);
		if(written != length)
		{
			if(error != nullptr)
			{
				//TODO add checking of errno
				*error = (String)"Unable to write all bytes to file stream";
			}
			return false;
		}
		
		if(std::fclose(file) == EOF)
		{
			if(error!=nullptr)
			{
				//TODO add checking of errno
				*error = "Error closing the file";
			}
			return false;
		}
		return true;
	}
	
	String Data::toString() const
	{
		if(data == nullptr)
		{
			return String();
		}
		else
		{
			return String((const char*)data, length);
		}
	}
	
	Data::operator String() const
	{
		if(data == nullptr)
		{
			return String();
		}
		else
		{
			return String((const char*)data, length);
		}
	}
	
	void* Data::getData(size_t byteIndex)
	{
		return (void*)(data+byteIndex);
	}

	const void* Data::getData(size_t byteIndex) const
	{
		return (const void*)(data+byteIndex);
	}

	void Data::assign(const void*data_arg, size_t size)
	{
		if(size > 0)
		{
			if(data_arg==nullptr)
			{
				throw IllegalArgumentException("data", "null");
			}
			
			if(data==nullptr)
			{
				data = (byte*)std::malloc(size);
			}
			else
			{
				data = (byte*)std::realloc(data, size);
			}

			const byte* copyBytes = (const byte*)data_arg;
			for(size_t i=0; i<size; i++)
			{
				data[i] = copyBytes[i];
			}
			length = size;
		}
		else
		{
			if(data != nullptr)
			{
				std::free(data);
				data = nullptr;
				length = 0;
			}
		}
	}
	
	void Data::assign(const Data& dataPacket)
	{
		if(dataPacket.length > 0)
		{
			if(data==nullptr)
			{
				data = (byte*)std::malloc(dataPacket.length);
			}
			else
			{
				data = (byte*)std::realloc(data, dataPacket.length);
			}

			for(size_t i=0; i<dataPacket.length; i++)
			{
				data[i] = dataPacket.data[i];
			}
			length = dataPacket.length;
		}
		else
		{
			if(data != nullptr)
			{
				std::free(data);
				data = nullptr;
				length = 0;
			}
		}
	}
	
	size_t Data::size() const
	{
		return length;
	}
	
	void Data::resize(size_t size)
	{
		if(size > 0)
		{
			if(data==nullptr)
			{
				data = (byte*)std::malloc(size);
				for(size_t i=0; i<size; i++)
				{
					data[i] = 0;
				}
				length = size;
			}
			else
			{
				data = (byte*)std::realloc(data, size);
				if(size>length)
				{
					for(size_t i=length; i<size; i++)
					{
						data[i] = 0;
					}
				}
				length = size;
			}
		}
		else
		{
			if(data!=nullptr)
			{
				std::free(data);
				data = nullptr;
				length = 0;
			}
		}
	}
	
	void Data::clear()
	{
		if(data!=nullptr)
		{
			std::free(data);
			data = nullptr;
			length = 0;
		}
	}
	
	void Data::add(byte dataByte)
	{
		if(data == nullptr)
		{
			data = (byte*)std::malloc(1);
			data[0] = dataByte;
			length=1;
		}
		else
		{
			data = (byte*)std::realloc(data, length+1);
			data[length] = dataByte;
			length++;
		}
	}

	void Data::add(size_t byteIndex, byte dataByte)
	{
		if(byteIndex > length)
		{
			throw DataOutOfBoundsException(byteIndex, length);
		}
		else
		{
			if(data == nullptr)
			{
				data = (byte*)std::malloc(1);
				data[0] = dataByte;
				length = 1;
			}
			else
			{
				size_t newLength = length+1;
				data = (byte*)std::realloc(data, newLength);
				for(size_t i=(newLength-1); i>byteIndex; i--)
				{
					data[i] = data[i-1];
				}
				data[byteIndex] = dataByte;
				length = newLength;
			}
		}
	}
	
	void Data::add(const void*data_arg, size_t size)
	{
		if(size>0)
		{
			if(data_arg == nullptr)
			{
				throw IllegalArgumentException("data", "null");
			}

			if(data == nullptr)
			{
				data = (byte*)std::malloc(size);
				const byte* copyBytes = (const byte*)data_arg;
				for(size_t i=0; i<size; i++)
				{
					data[i] = copyBytes[i];
				}
				length = size;
			}
			else
			{
				size_t newLength = length + size;
				data = (byte*)std::realloc(data, newLength);
				const byte*copyBytes = (const byte*)data_arg;
				size_t counter = 0;
				for(size_t i=length; i<newLength; i++)
				{
					data[i] = copyBytes[counter];
					counter++;
				}
				length = newLength;
			}
		}
	}
	
	void Data::add(size_t byteIndex, const void* data_arg, size_t size)
	{
		if(data_arg == nullptr)
		{
			throw IllegalArgumentException("data","null");
		}
		else if(byteIndex > length)
		{
			throw DataOutOfBoundsException(byteIndex, length);
		}
		else if(size>0)
		{
			if(data == nullptr)
			{
				data = (byte*)std::malloc(size);
				const byte* copyBytes = (const byte*)data_arg;
				for(size_t i=0; i<size; i++)
				{
					data[i] = copyBytes[i];
				}
				length = size;
			}
			else
			{
				size_t newLength = length+size;
				data = (byte*)std::realloc(data, newLength);
				const byte* copyBytes = (const byte*)data_arg;
				size_t counter = (length-1);
				size_t endIndex = byteIndex + size;
				for(size_t i=(newLength-1); i>endIndex; i--)
				{
					data[i] = data[counter];
					counter--;
				}
				counter = byteIndex;
				for(size_t i=0; i<size; i++)
				{
					data[counter] = copyBytes[i];
					counter++;
				}
				length = newLength;
			}
		}
	}
	
	void Data::add(const Data& dataPacket)
	{
		if(dataPacket.length>0)
		{
			if(data == nullptr)
			{
				data = (byte*)std::malloc(dataPacket.length);
				for(size_t i=0; i<dataPacket.length; i++)
				{
					data[i] = dataPacket.data[i];
				}
				length = dataPacket.length;
			}
			else
			{
				size_t newLength = length + dataPacket.length;
				data = (byte*)std::realloc(data, newLength);
				size_t counter = 0;
				for(size_t i=length; i<newLength; i++)
				{
					data[i] = dataPacket.data[counter];
					counter++;
				}
				length = newLength;
			}
		}
	}
	
	void Data::add(size_t byteIndex, const Data& dataPacket)
	{
		if(byteIndex > length)
		{
			throw DataOutOfBoundsException(byteIndex, length);
		}
		else if(dataPacket.length>0)
		{
			if(data == nullptr)
			{
				data = (byte*)std::malloc(dataPacket.length);
				for(size_t i=0; i<dataPacket.length; i++)
				{
					data[i] = dataPacket.data[i];
				}
				length = dataPacket.length;
			}
			else
			{
				size_t newLength = length+dataPacket.length;
				data = (byte*)std::realloc(data, newLength);
				size_t counter = (length-1);
				size_t endIndex = byteIndex + dataPacket.length;
				for(size_t i=(newLength-1); i>endIndex; i--)
				{
					data[i] = data[counter];
					counter--;
				}
				counter = byteIndex;
				for(size_t i=0; i<dataPacket.length; i++)
				{
					data[counter] = dataPacket.data[i];
					counter++;
				}
				length = newLength;
			}
		}
	}
	
	void Data::remove(size_t byteIndex, size_t size)
	{
		if(byteIndex >= length)
		{
			throw DataOutOfBoundsException(byteIndex, length);
		}
		else if(size>0)
		{
			size_t endOfRemove = byteIndex+size;
			size_t counter = byteIndex;
			for(size_t i=endOfRemove; i<length; i++)
			{
				data[counter] = data[i];
				counter++;
			}
			size_t newLength = length - size;
			if(newLength == 0)
			{
				std::free(data);
				data = nullptr;
				length = 0;
			}
			else
			{
				data = (byte*)std::realloc(data, newLength);
				length = newLength;
			}
		}
	}
}
