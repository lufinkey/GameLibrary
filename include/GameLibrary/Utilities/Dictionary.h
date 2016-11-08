
#pragma once

#include "Any.h"
#include "ArrayList.h"
#include "Data.h"
#include "Pair.h"
#include "String.h"

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
		BasicDictionary(const BasicDictionary&);
		BasicDictionary(BasicDictionary&&);
		BasicDictionary(const ArrayList<KEY_TYPE>& keys, const ArrayList<VALUE_TYPE>& values);
		BasicDictionary(const ArrayList<Pair<KEY_TYPE, VALUE_TYPE> >& contents);
		BasicDictionary(ArrayList<Pair<KEY_TYPE, VALUE_TYPE> >&& contents);
		~BasicDictionary();
		
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
			Any& value = dict.get(key);
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
}

#include "Dictionary.impl"
