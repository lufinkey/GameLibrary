
#include <GameLibrary/Utilities/Plist.h>
#include <GameLibrary/Types.h>
#include <GameLibrary/Utilities/PlatformChecks.h>
#include <GameLibrary/Exception/IllegalArgumentException.h>
#include <pugixml.hpp>
#include <base64.hpp>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <utility>

//loading and saving plist code adapted from PlistCpp. https://github.com/animetrics/PlistCpp
//All credit goes to animetrics

namespace fgl
{
	void Plist_base64Decode(const char* encodedData, std::vector<char>& data);
	void Plist_base64Encode(std::string& dataEncoded, const Data& data);
	
	Pair<size_t, size_t> Plist_getParsePosition(const void* ptr, size_t offset);
	bool Plist_parse(Any* dst, const void* ptr, pugi::xml_node& node, String* error);
	bool Plist_parseDictionary(Plist::dict* dst, const void* ptr, pugi::xml_node&node, String* error);
	bool Plist_parseArray(Plist::array* dst, const void* ptr, pugi::xml_node& node, String* error);
	bool Plist_parseDate(Plist::date* dst, const void* ptr, pugi::xml_node& node, String* error);
	void Plist_parse_error(const void* ptr, pugi::xml_node& node, const String& error_message, String* error);
	
	bool Plist_write(const Any& src, pugi::xml_node& node, String* error);
	bool Plist_writeDictionary(const Plist::dict& src, pugi::xml_node& node, String* error);
	template<typename VALUE_TYPE>
	bool Plist_writeArray(const ArrayList<VALUE_TYPE>& src, pugi::xml_node& node, String* error);
	
	
	
	bool Plist::loadFromFile(Dictionary* dst, const String& path, String* error)
	{
		if(dst==nullptr)
		{
			throw IllegalArgumentException("dst", "cannot be null");
		}
		
		//load the file into memory
		FILE*file = std::fopen(path, "rb");
		if(file == nullptr)
		{
			//TODO add switch for errno
			if(error!=nullptr)
			{
				*error = "Unable to load plist from file";
			}
			return false;
		}
		std::fseek(file, 0, SEEK_END);
		long fileSize = std::ftell(file);
		std::fseek(file, 0, SEEK_SET);
		if(fileSize < 0)
		{
			std::fclose(file);
			throw Exception("Unknown error. File size was negative");
		}
		else if(fileSize==0)
		{
			if(error!=nullptr)
			{
				*error = "File is empty";
			}
			return false;
		}
		char* fileContent = (char*)std::malloc((size_t)fileSize);
		if(fileContent==nullptr)
		{
			std::fclose(file);
			throw std::bad_alloc();
		}
		std::fread((void*)fileContent, 1, (size_t)fileSize, file);
		std::fclose(file);
		
		//parse the data
		bool success = loadFromPointer(dst, (const void*)fileContent, (size_t)fileSize, error);
		std::free(fileContent);
		return success;
	}
	
	bool Plist::loadFromData(Dictionary* dst, const Data& data, String* error)
	{
		if(dst==nullptr)
		{
			throw IllegalArgumentException("dst", "cannot be null");
		}
		else if(data.size()==0)
		{
			throw IllegalArgumentException("data", "cannot be empty");
		}
		return loadFromPointer(dst, data.getData(), data.size(), error);
	}
	
	bool Plist::loadFromString(Dictionary* dst, const String& string, String* error)
	{
		if(dst==nullptr)
		{
			throw IllegalArgumentException("dst", "cannot be null");
		}
		else if(string.length()==0)
		{
			throw IllegalArgumentException("string", "cannot be empty");
		}
		return loadFromPointer(dst, (const void*)string.getData(), string.length(), error);
	}
	
	bool Plist::loadFromPointer(Dictionary* dst, const void* pointer, size_t length, String* error)
	{
		if(dst==nullptr)
		{
			throw IllegalArgumentException("dst", "cannot be null");
		}
		else if(pointer==nullptr)
		{
			throw IllegalArgumentException("pointer", "cannot be null");
		}
		else if(length==0)
		{
			if(error!=nullptr)
			{
				*error = "pointer is empty";
			}
			return false;
		}
		// infer plist type from header.  If it has the bplist00 header as first 8
		// bytes, then it's a binary plist.  Otherwise, assume it's XML
		if(length>8 && String((const char*)pointer, 8).equals("bplist00"))
		{
			//binary plist
			if(error!=nullptr)
			{
				*error = "Binary plists are not yet supported";
			}
			return false;
		}
		else
		{
			//XML plist
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(pointer, length, pugi::parse_default, pugi::xml_encoding::encoding_auto);
			if(!result)
			{
				if(error!=nullptr)
				{
					*error = String("pugixml: ")+result.description();
				}
				return false;
			}
			pugi::xml_node plistNode = doc.child("plist");
			if(plistNode.type()==pugi::node_null)
			{
				if(error!=nullptr)
				{
					*error = "root element is not \"plist\"";
				}
				return false;
			}
			dst->clear();
			pugi::xml_node rootNode = plistNode.first_child();
			if(rootNode.type()==pugi::node_null)
			{
				//plist node is empty
				return true;
			}
			else if(String::streq(rootNode.name(), "dict"))
			{
				return Plist_parseDictionary(dst, pointer, rootNode, error);
			}
			else
			{
				if(error!=nullptr)
				{
					*error = "root of \"plist\" element is not a \"dict\" element";
				}
				return false;
			}
		}
	}
	
	class Plist_writer : public pugi::xml_writer
	{
	public:
		FILE* file;
		String* error;
		bool success;
		
		Plist_writer(String* error) : file(nullptr), error(error), success(true)
		{
			//
		}
		
		bool open(const String& path, String* error)
		{
			file = std::fopen(path, "wb");
			if(file==nullptr)
			{
				if(error != nullptr)
				{
					//TODO add checking of errno
					*error = "Unable to open file for writing";
				}
				return false;
			}
		}
		
		void close()
		{
			std::fclose(file);
		}

		virtual void write(const void* data, size_t size) override
		{
			size_t bytesWritten = std::fwrite(data, 1, size, file);
			if(bytesWritten < size)
			{
				if(success)
				{
					success = false;
					if(error!=nullptr)
					{
						*error = "Unable to write all bytes to file stream";
					}
				}
			}
		}
	};
	
	bool Plist::saveToFile(const Dictionary& src, const String& path, String* error)
	{
		pugi::xml_document doc;
		pugi::xml_node decNode = doc.append_child(pugi::node_declaration);
		decNode.append_attribute("version") = "1.0";
		decNode.append_attribute("encoding") = "UTF-8";
		
		// doctype node
		doc.append_child(pugi::node_doctype).set_value("plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");

		// root node
		pugi::xml_node plistNode = doc.append_child("plist");
		plistNode.append_attribute("version") = "1.0";

		bool success = Plist_writeDictionary(src, plistNode, error);
		if(success)
		{
			Plist_writer writer(error);
			success = writer.open(path, error);
			if(!success)
			{
				return false;
			}
			doc.save(writer);
			writer.close();
			return writer.success;
		}
		return false;
	}
	
	void Plist_base64Decode(const char* encodedData, std::vector<char>& data)
	{
		std::insert_iterator<std::vector<char> > ii(data, data.begin());
		base64<char> b64;
		int state = 0;
		b64.get(encodedData, encodedData + strlen(encodedData), ii, state);
	}

	void Plist_base64Encode(std::string& dataEncoded, const Data& data)
	{
		const char* str = (const char*)data.getData();
		std::vector<char> data_vect(str, str+data.size());
		dataEncoded.clear();
		std::insert_iterator<std::string> ii(dataEncoded, dataEncoded.begin());
		base64<char> b64;
		int state = 0;
		
		#ifdef TARGETPLATFORM_WINDOWS
			b64.put(data_vect.begin(), data_vect.end(), ii, state, base64<>::crlf());
		#else
			b64.put(data_vect.begin(), data_vect.end(), ii, state, base64<>::lf());
		#endif
	}
	
	Pair<size_t, size_t> Plist_getParsePosition(const void* ptr, size_t offset)
	{
		size_t currentLine = 1;
		size_t currentOffset = 0;
		const char* str = (const char*)ptr;
		bool didNewline = false;
		for(size_t i = 0; i < offset; i++)
		{
			if(str[i]=='\n')
			{
				currentLine++;
				currentOffset = 0;
				didNewline = true;
			}
			else if(str[i] == '\r')
			{
				if(!didNewline)
				{
					currentLine++;
					currentOffset = 0;
				}
			}
			else
			{
				currentOffset++;
			}
			if(didNewline)
			{
				didNewline = false;
			}
		}
		return Pair<size_t, size_t>(currentLine, currentOffset);
	}
	
	bool Plist_parse(Any* dst, const void* ptr, pugi::xml_node& node, String* error)
	{
		const char* nodeName = node.name();
		if(String::streq(nodeName, "dict"))
		{
			*dst = Plist::dict();
			return Plist_parseDictionary(&dst->as<Plist::dict>(), ptr, node, error);
		}
		else if(String::streq(nodeName, "array"))
		{
			*dst = Plist::array();
			return Plist_parseArray(&dst->as<Plist::array>(), ptr, node, error);
		}
		else if(String::streq(nodeName, "string"))
		{
			*dst = Plist::string(node.first_child().value());
			return true;
		}
		else if(String::streq(nodeName, "data"))
		{
			const char* data_str = (const char*)node.first_child().value();
			if(data_str==nullptr)
			{
				*dst = Plist::data();
			}
			else
			{
				std::vector<char> data_vect;
				Plist_base64Decode(data_str, data_vect);
				*dst = Plist::data((const void*)data_vect.data(), data_vect.size());
			}
			return true;
		}
		else if(String::streq(nodeName, "data"))
		{
			*dst = Plist::date();
			return Plist_parseDate(&dst->as<Plist::date>(), ptr, node, error);
		}
		else if(String::streq(nodeName, "integer"))
		{
			try
			{
				const char* str = node.first_child().value();
				if(str==nullptr || str[0]=='\0')
				{
					*dst = Plist::integer(0LL);
				}
				else
				{
					*dst = Plist::integer(String(str).toArithmeticValue<long long>());
				}
			}
			catch(const NumberFormatException& e)
			{
				Plist_parse_error(ptr, node, e.message, error);
				return false;
			}
			return true;
		}
		else if(String::streq(nodeName, "real"))
		{
			try
			{
				const char* str = node.first_child().value();
				if(str==nullptr || str[0]=='\0')
				{
					*dst = Plist::real(0LL);
				}
				else
				{
					*dst = Plist::real(String(str).toArithmeticValue<long double>());
				}
			}
			catch(const NumberFormatException& e)
			{
				Plist_parse_error(ptr, node, e.message, error);
				return false;
			}
			return true;
		}
		else if(String::streq(nodeName, "boolean"))
		{
			bool val = false;
			const char* str = node.first_child().value();
			if(str==nullptr)
			{
				val = false;
			}
			else if(String::streq(str,"true") || String::streq(str,"TRUE") || String::streq(str,"yes") || String::streq(str,"YES") || String::streq(str,"1"))
			{
				val = true;
			}
			else if(String::streq(str,"false") || String::streq(str,"FALSE") || String::streq(str,"no") || String::streq(str,"NO") || String::streq(str,"0"))
			{
				val = false;
			}
			else
			{
				Plist_parse_error(ptr, node, "Invalid boolean value", error);
				return false;
			}
			*dst = Plist::boolean(val);
			return true;
		}
		else if(String::streq(nodeName,"true"))
		{
			*dst = Plist::boolean(true);
			return true;
		}
		else if(String::streq(nodeName,"false"))
		{
			*dst = Plist::boolean(false);
			return true;
		}
		else
		{
			Plist_parse_error(ptr, node, (String)"Invalid tag name \""+ nodeName+"\"", error);
			return false;
		}
	}
	
	bool Plist_parseDictionary(Plist::dict* dst, const void* ptr, pugi::xml_node&node, String* error)
	{
		char empty_key[1] = {'\0'};
		size_t size = std::distance(node.begin(), node.end())/2;
		dst->reserve(size);
		for(pugi::xml_node_iterator it=node.begin(); it!=node.end(); it++)
		{
			if(!String::streq(it->name(), "key"))
			{
				Plist_parse_error(ptr, node, "plist \"key\" element expected but not found", error);
				return false;
			}
			
			const char* key_str = it->first_child().value();
			if(key_str==nullptr)
			{
				key_str = empty_key;
			}
			Plist::key key(key_str);
			pugi::xml_node& key_node = *it;
			it++;

			if(it == node.end())
			{
				Plist_parse_error(ptr, key_node, "element after \"key\" element expected but not found", error);
				return false;
			}
			else if(String::streq(it->name(), "key"))
			{
				Plist_parse_error(ptr, *it, "\"key\" element after \"key\" element is not valid", error);
				return false;
			}
			Any value;
			bool result = Plist_parse(&value, ptr, *it, error);
			if(!value.empty())
			{
				dst->set(key, std::move(value));
			}
			if(!result)
			{
				return false;
			}
		}
		return true;
	}
	
	bool Plist_parseArray(Plist::array* dst, const void* ptr, pugi::xml_node& node, String* error)
	{
		size_t size = std::distance(node.begin(), node.end());
		dst->reserve(size);
		for(pugi::xml_node_iterator it=node.begin(); it!=node.end(); it++)
		{
			Any value;
			bool result = Plist_parse(&value, ptr, *it, error);
			if(!value.empty())
			{
				dst->add(std::move(value));
			}
			if(!result)
			{
				return false;
			}
		}
		return true;
	}
	
	bool Plist_parseDate(Plist::date* dst, const void* ptr, pugi::xml_node& node, String* error)
	{
		const char* date_str = node.first_child().value();
		if(date_str==nullptr || date_str[0]=='\0')
		{
			Plist_parse_error(ptr, node, "\"date\" element is empty", error);
			return false;
		}
		int month=0, day=0, year=0, hour24=0, minute=0, second=0;
		// parse date string.  E.g.  2011-09-25T02:31:04Z
		int args = sscanf(date_str, "%4d-%2d-%2dT%2d:%2d:%2dZ", &year, &month, &day, &hour24, &minute, &second);
		if(args < 6)
		{
			Plist_parse_error(ptr, node, "Invalid datetime format", error);
			return false;
		}
		struct tm tmTime;
		tmTime.tm_year = year - 1900;
		tmTime.tm_mon = month - 1;
		tmTime.tm_mday = day;
		tmTime.tm_hour = hour24;
		tmTime.tm_min = minute;
		tmTime.tm_sec = second;
		tmTime.tm_wday = 0;
		tmTime.tm_yday = 0;
		tmTime.tm_isdst = -1;
		#ifdef _WIN32
			time_t timeval = _mkgmtime(&tmTime);
		#else
			time_t timeval = timegm(&tmTime);
		#endif
		if(timeval==-1)
		{
			Plist_parse_error(ptr, node, "Invalid datetime values", error);
			return false;
		}
		dst->assign(timeval);
		return true;
	}
	
	void Plist_parse_error(const void* ptr, pugi::xml_node& node, const String& error_message, String* error)
	{
		size_t offset = node.offset_debug();
		Pair<size_t, size_t> doc_pos = Plist_getParsePosition(ptr, offset);
		if(error!=nullptr)
		{
			*error = (String)"" + doc_pos.first + ":" + doc_pos.second + ": " + error_message;
		}
	}
	
	bool Plist_write(const Any& src, pugi::xml_node& node, String* error)
	{
		if(src.is<Plist::dict>())
		{
			const Plist::dict& dict = src.as<Plist::dict>();
			return Plist_writeDictionary(dict, node, error);
		}
		else if(src.is<Plist::array>())
		{
			const Plist::array& array = src.as<Plist::array>();
			return Plist_writeArray(array, node, error);
		}
		else if(src.is<Plist::string>())
		{
			node.append_child("string").append_child(pugi::node_pcdata).set_value(src.as<Plist::string>());
			return true;
		}
		else if(src.is<Plist::data>())
		{
			const Plist::data& data = src.as<Plist::data>();
			std::string encoded_str;
			Plist_base64Encode(encoded_str, data);
			node.append_child("data").append_child(pugi::node_pcdata).set_value(encoded_str.c_str());
			return true;
		}
		else if(src.is<Plist::date>())
		{
			node.append_child("date").append_child(pugi::node_pcdata).set_value(src.as<Plist::date>().toISO8601String());
			return true;
		}
		else if(src.is<Number>()) //Plist::integer, Plist::real, Plist::boolean
		{
			const Number& number = src.as<Number>();
			if(number.isBool())
			{
				bool val = number.toArithmeticValue<bool>();
				if(val)
				{
					node.append_child("true");
				}
				else
				{
					node.append_child("false");
				}
				return true;
			}
			else if(number.isIntegral())
			{
				node.append_child("integer").append_child(pugi::node_pcdata).set_value(number.toString());
				return true;
			}
			else //if(number.isFloatingPoint())
			{
				node.append_child("real").append_child(pugi::node_pcdata).set_value(number.toString());
				return true;
			}
		}
		else if(src.is<WideString>())
		{
			node.append_child("string").append_child(pugi::node_pcdata).set_value(src.as<WideString>().toBasicString<char>());
			return true;
		}
		else if(src.is<std::string>())
		{
			node.append_child("string").append_child(pugi::node_pcdata).set_value(src.as<std::string>().c_str());
			return true;
		}
		else if(src.is<const char*>())
		{
			node.append_child("string").append_child(pugi::node_pcdata).set_value(src.as<const char*>());
			return true;
		}
		else if(src.is<fgl::Int64>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Int64>());
			return true;
		}
		else if(src.is<fgl::Int32>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Int32>());
			return true;
		}
		else if(src.is<fgl::Int16>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Int16>());
			return true;
		}
		else if(src.is<fgl::Int8>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Int8>());
			return true;
		}
		else if(src.is<long double>())
		{
			node.append_child("real").append_child(pugi::node_pcdata).set_value((String)"" + src.as<long double>());
			return true;
		}
		else if(src.is<double>())
		{
			node.append_child("real").append_child(pugi::node_pcdata).set_value((String)"" + src.as<double>());
			return true;
		}
		else if(src.is<float>())
		{
			node.append_child("real").append_child(pugi::node_pcdata).set_value((String)"" + src.as<float>());
			return true;
		}
		else if(src.is<bool>())
		{
			bool val = src.as<bool>();
			if(val)
			{
				node.append_child("true");
			}
			else
			{
				node.append_child("false");
			}
			return true;
		}
		else if(src.is<fgl::Uint64>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Uint64>());
			return true;
		}
		else if(src.is<fgl::Uint32>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Uint32>());
			return true;
		}
		else if(src.is<fgl::Uint16>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Uint16>());
			return true;
		}
		else if(src.is<fgl::Uint8>())
		{
			node.append_child("integer").append_child(pugi::node_pcdata).set_value((String)"" + src.as<fgl::Uint8>());
			return true;
		}
		else
		{
			node.append_child("string").append_child(pugi::node_pcdata).set_value(src.toString());
			return true;
		}
	}
	
	bool Plist_writeDictionary(const Plist::dict& src, pugi::xml_node& node, String* error)
	{
		pugi::xml_node dict_node = node.append_child("dict");
		const ArrayList<Pair<Plist::key, Any> >& contents = src.getContents();
		for(size_t contents_size=contents.size(), i=0; i<contents_size; i++)
		{
			const Pair<Plist::key, Any>& pair = contents.get(i);
			pugi::xml_node key_node = dict_node.append_child("key");
			key_node.append_child(pugi::node_pcdata).set_value(pair.first);
			bool result = Plist_write(pair.second, dict_node, error);
			if(!result)
			{
				return false;
			}
		}
		return true;
	}
	
	template<typename VALUE_TYPE>
	bool Plist_writeArray(const ArrayList<VALUE_TYPE>& src, pugi::xml_node& node, String* error)
	{
		pugi::xml_node array_node = node.append_child("array");
		for(size_t array_size=src.size(), i=0; i<array_size; i++)
		{
			bool result = Plist_write(src[i], array_node, error);
			if(!result)
			{
				return false;
			}
		}
		return true;
	}
}
