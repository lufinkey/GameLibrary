
#pragma once

#include "Any.hpp"
#include "ArrayList.hpp"
#include "Data.hpp"
#include "Pair.hpp"
#include "String.hpp"
#include <GameLibrary/Exception/Utilities/DictionaryKeyNotFoundException.hpp>

namespace fgl
{
	template<typename KEY_TYPE, typename VALUE_TYPE>
	class BasicDictionary
	{
	public:
		typedef KEY_TYPE key_type;
		typedef VALUE_TYPE value_type;
		typedef BasicDictionary<KEY_TYPE, VALUE_TYPE> BasicDictionary_type;
		
		class ValueProxy
		{
			friend class BasicDictionary<KEY_TYPE, VALUE_TYPE>;
		public:
			ValueProxy() = delete;
			ValueProxy(const ValueProxy&) = delete;
			ValueProxy& operator=(const ValueProxy&) = delete;
			
			VALUE_TYPE& operator=(VALUE_TYPE&);
			VALUE_TYPE& operator=(VALUE_TYPE&&);
			VALUE_TYPE& operator=(const VALUE_TYPE&);
			VALUE_TYPE& operator=(const VALUE_TYPE&&);
			
			operator VALUE_TYPE&();
			
		private:
			KEY_TYPE key;
			BasicDictionary<KEY_TYPE, VALUE_TYPE>& dictionary;
			
			ValueProxy(ValueProxy&&);
			ValueProxy(const KEY_TYPE& key, BasicDictionary<KEY_TYPE, VALUE_TYPE>& dictionary);
		};
		
		BasicDictionary();
		BasicDictionary(const ArrayList<KEY_TYPE>& keys, const ArrayList<VALUE_TYPE>& values);
		BasicDictionary(const ArrayList<Pair<KEY_TYPE, VALUE_TYPE> >& contents);
		BasicDictionary(ArrayList<Pair<KEY_TYPE, VALUE_TYPE> >&& contents);
		~BasicDictionary();

		bool has(const KEY_TYPE& key) const;
		
		VALUE_TYPE& set(const KEY_TYPE& key, VALUE_TYPE& value);
		VALUE_TYPE& set(const KEY_TYPE& key, VALUE_TYPE&& value);
		VALUE_TYPE& set(const KEY_TYPE& key, const VALUE_TYPE& value);
		VALUE_TYPE& set(const KEY_TYPE& key, const VALUE_TYPE&& value);
		
		VALUE_TYPE& get(const KEY_TYPE& key);
		const VALUE_TYPE& get(const KEY_TYPE& key) const;
		VALUE_TYPE& get(const KEY_TYPE& key, VALUE_TYPE& defaultValue);
		const VALUE_TYPE& get(const KEY_TYPE& key, const VALUE_TYPE& defaultValue) const;
		
		ValueProxy operator[](const KEY_TYPE& key);
		
		ArrayList<KEY_TYPE> getKeys() const;
		ArrayList<VALUE_TYPE> getValues() const;
		const ArrayList<Pair<KEY_TYPE, VALUE_TYPE> >& getContents() const;
		
		size_t size() const;
		void clear();
		void reserve(size_t size);

		bool isEmpty() const;
		
		BasicDictionary<KEY_TYPE, VALUE_TYPE> filter(std::function<bool(const KEY_TYPE& key, const VALUE_TYPE& value)> func) const;
		
		String toString() const;
		
	private:
		ArrayList<Pair<KEY_TYPE, VALUE_TYPE> > contents;
		
		size_t indexOfKey(const KEY_TYPE& key) const;
	};
	
	typedef BasicDictionary<String, Any> Dictionary;

	template<typename RESULT_TYPE>
	RESULT_TYPE extract(const Dictionary& dict, const Dictionary::key_type& key)
	{
		try
		{
			const Any& value = dict.get(key);
			try
			{
				return value.as<RESULT_TYPE>();
			}
			catch(const BadAnyCastException&)
			{
				return RESULT_TYPE();
			}
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return RESULT_TYPE();
		}
	}

	template<typename RESULT_TYPE>
	RESULT_TYPE extract(const Dictionary& dict, const Dictionary::key_type& key, const RESULT_TYPE& defaultValue)
	{
		try
		{
			const Any& value = dict.get(key);
			try
			{
				return value.as<RESULT_TYPE>();
			}
			catch(const BadAnyCastException&)
			{
				return defaultValue;
			}
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return defaultValue;
		}
	}

	template<typename RESULT_TYPE>
	RESULT_TYPE extract(const ArrayList<fgl::Any>& array, size_t index)
	{
		const Any& value = array.get(index);
		try
		{
			return value.as<RESULT_TYPE>();
		}
		catch(const BadAnyCastException&)
		{
			return RESULT_TYPE();
		}
	}

	template<typename RESULT_TYPE>
	RESULT_TYPE extract(const ArrayList<fgl::Any>& array, size_t index, const RESULT_TYPE& defaultValue)
	{
		const Any& value = array.get(index);
		try
		{
			return value.as<RESULT_TYPE>();
		}
		catch(const BadAnyCastException&)
		{
			return defaultValue;
		}
	}
}

#include "Dictionary.impl"
