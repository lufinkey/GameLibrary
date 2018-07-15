
#pragma once

#include <map>
#include <utility>
#include "Any.hpp"
#include "ArrayList.hpp"
#include "Data.hpp"
#include "String.hpp"
#include "TypeRegistry.hpp"
#include <GameLibrary/Exception/Utilities/DictionaryKeyNotFoundException.hpp>

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

namespace fgl
{
	template<typename KEY_TYPE, typename VALUE_TYPE>
	class BasicDictionary
	{
	public:
		typedef KEY_TYPE Key;
		typedef VALUE_TYPE Value;
		typedef std::pair<KEY_TYPE, VALUE_TYPE> Entry;
		
		typedef typename ArrayList<Entry>::iterator iterator;
		typedef typename ArrayList<Entry>::const_iterator const_iterator;
		typedef typename ArrayList<Entry>::reverse_iterator reverse_iterator;
		typedef typename ArrayList<Entry>::const_reverse_iterator const_reverse_iterator;
		
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
		BasicDictionary(std::initializer_list<std::pair<KEY_TYPE, VALUE_TYPE>> list);
		BasicDictionary(const std::map<KEY_TYPE, VALUE_TYPE>& map);
		BasicDictionary(const ArrayList<KEY_TYPE>& keys, const ArrayList<VALUE_TYPE>& values);
		BasicDictionary(const ArrayList<std::pair<KEY_TYPE, VALUE_TYPE> >& contents);
		BasicDictionary(ArrayList<std::pair<KEY_TYPE, VALUE_TYPE> >&& contents);
		~BasicDictionary();
		
		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		bool has(const KEY_TYPE& key) const;
		size_t indexOfKey(const KEY_TYPE& key) const;
		
		VALUE_TYPE& set(const KEY_TYPE& key, VALUE_TYPE& value);
		VALUE_TYPE& set(const KEY_TYPE& key, VALUE_TYPE&& value);
		VALUE_TYPE& set(const KEY_TYPE& key, const VALUE_TYPE& value);
		VALUE_TYPE& set(const KEY_TYPE& key, const VALUE_TYPE&& value);
		
		VALUE_TYPE& get(const KEY_TYPE& key);
		const VALUE_TYPE& get(const KEY_TYPE& key) const;
		VALUE_TYPE& get(const KEY_TYPE& key, VALUE_TYPE& defaultValue);
		const VALUE_TYPE& get(const KEY_TYPE& key, const VALUE_TYPE& defaultValue) const;

		VALUE_TYPE& valueAt(size_t index);
		const VALUE_TYPE& valueAt(size_t index) const;
		
		void remove(const KEY_TYPE& key);
		
		ValueProxy operator[](const KEY_TYPE& key);
		
		ArrayList<KEY_TYPE> getKeys() const;
		ArrayList<VALUE_TYPE> getValues() const;
		const ArrayList<std::pair<KEY_TYPE, VALUE_TYPE>>& getContents() const;
		
		size_t size() const;
		void clear();
		void reserve(size_t size);

		bool isEmpty() const;
		
		BasicDictionary<KEY_TYPE, VALUE_TYPE> filter(const std::function<bool(const KEY_TYPE& key, const VALUE_TYPE& value)>& func) const;
		#ifdef __OBJC__
		BasicDictionary<KEY_TYPE, VALUE_TYPE> filter(BOOL(^func)(const KEY_TYPE& key, const VALUE_TYPE& value)) const;
		#endif
		
		void forEach(const std::function<void(const KEY_TYPE& key, VALUE_TYPE& value)>& func);
		void forEach(const std::function<void(const KEY_TYPE& key, const VALUE_TYPE& value)>& func) const;
		#ifdef __OBJC__
		void forEach(void(^func)(const KEY_TYPE& key, VALUE_TYPE& value));
		void forEach(void(^func)(const KEY_TYPE& key, const VALUE_TYPE& value)) const;
		#endif
		
		String toString() const;
		
	private:
		ArrayList<std::pair<KEY_TYPE, VALUE_TYPE> > contents;
	};
	
	typedef BasicDictionary<String, Any> Dictionary;

	template<typename RESULT_TYPE>
	RESULT_TYPE extract(const Dictionary& dict, const Dictionary::Key& key)
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
	RESULT_TYPE extract(const Dictionary& dict, const Dictionary::Key& key, const RESULT_TYPE& defaultValue)
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

REGISTER_TYPE(fgl::Dictionary)
