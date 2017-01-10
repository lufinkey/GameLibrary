
#pragma once

#include "Any.hpp"
#include "ArrayList.hpp"
#include "Data.hpp"
#include "Dictionary.hpp"
#include "Number.hpp"
#include "String.hpp"
#include "Time/DateTime.hpp"

namespace fgl
{
	class Plist
	{
	public:
		typedef String key;
		typedef BasicDictionary<Plist::key, Any> dict;
		typedef ArrayList<Any> array;
		typedef String string;
		typedef Data data;
		typedef DateTime date;
		typedef Number integer;
		typedef Number real;
		typedef Number boolean;
		
		static bool loadFromFile(Dictionary* dst, const String& path, String* error=nullptr);
		static bool loadFromData(Dictionary* dst, const Data& data, String* error=nullptr);
		static bool loadFromString(Dictionary* dst, const String& string, String* error=nullptr);
		static bool loadFromPointer(Dictionary* dst, const void* pointer, size_t length, String* error=nullptr);
		
		static bool saveToFile(const Dictionary& src, const String& path, String* error=nullptr);
	};
}
