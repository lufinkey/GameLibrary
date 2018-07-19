
#pragma once

#include <cstdio>
#include <GameLibrary/Types.hpp>
#include "String.hpp"

namespace fgl
{
	//this class is horribly written. I know. I wrote it like 2 years ago. I'll rewrite it later
	class Data
	{
	private:
		byte* data;
		size_t length;
		
	public:
		Data();
		Data(const Data&);
		Data(Data&&);
		explicit Data(const String&);
		explicit Data(size_t size);
		Data(const void* data, size_t size);
		~Data();
		
		Data& operator=(const Data&);
		Data& operator=(Data&&);
		
		bool loadFromPath(const String& path, String* error=nullptr);
		bool loadFromFile(FILE* file, String* error=nullptr);
		bool saveToPath(const String& path, String* error=nullptr) const;
		
		String toString() const;
		explicit operator String() const;
		
		void* getData(size_t byteIndex=0);
		const void* getData(size_t byteIndex=0) const;
		void assign(const void* data, size_t size);
		void assign(const Data& data);
		size_t size() const;
		
		void resize(size_t size);
		void clear();
		
		void add(byte dataByte);
		void add(size_t byteIndex, byte dataByte);
		void add(const void* data, size_t size);
		void add(size_t byteIndex, const void* data, size_t size);
		void add(const Data& data);
		void add(size_t byteIndex, const Data& data);
		
		void remove(size_t byteIndex, size_t size=1);
	};
}
