
#pragma once

#include <array>
#include <climits>
#include <initializer_list>
#include <type_traits>
#include <vector>

#ifdef _ARRAYLIST_STANDALONE
#include <stdexcept>
#else
#include <GameLibrary/Exception/Utilities/ArrayListOutOfBoundsException.hpp>
#include "Stringifier.hpp"

namespace fgl
{
#endif
	//Mostly just a wrapper around std::vector
	template <typename T>
	class ArrayList
	{
	private:
		std::vector<T> objects;
		
	public:
		static constexpr size_t NOT_FOUND = (size_t)-1;
		typedef T value_type;
		
		ArrayList()
		{
			//
		}
		
		ArrayList(const ArrayList<T>& arr)
			: objects(arr.objects)
		{
			//
		}
		
		ArrayList(ArrayList<T>&& arr)
			: objects(arr.objects)
		{
			//
		}
		
		template<size_t SIZE>
		ArrayList(const std::array<T, SIZE>& arr)
			: objects()
		{
			objects.assign(arr.begin(), arr.end());
		}
		
		ArrayList(const std::vector<T>& vect)
			: objects(vect)
		{
			//
		}
		
		ArrayList(std::initializer_list<T> list)
			: objects(list)
		{
			//
		}
		
		explicit ArrayList(size_t size)
		{
			objects.resize(size);
		}
		
		ArrayList(const T* data, size_t size)
		{
			objects.assign(data, data+size);
		}
		
		~ArrayList()
		{
			//
		}
		
		ArrayList<T>& operator=(const ArrayList<T>& arr)
		{
			objects = arr.objects;
			objects.shrink_to_fit();
			return *this;
		}
		
		ArrayList<T>& operator=(const std::vector<T>& vect)
		{
			objects = vect;
			objects.shrink_to_fit();
			return *this;
		}
		
		T& operator[] (size_t index)
		{
			if(index<objects.size())
			{
				return objects[index];
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		const T& operator[] (size_t index) const
		{
			if(index<objects.size())
			{
				return objects[index];
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		T& get(size_t index)
		{
			if(index < objects.size())
			{
				return objects[index];
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		const T& get(size_t index) const
		{
			if(index<objects.size())
			{
				return objects[index];
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		T* getData()
		{
			return objects.data();
		}

		const T* getData() const
		{
			return objects.data();
		}
		
		void set(size_t index, const T& obj)
		{
			if(index < objects.size())
			{
				objects[index] = obj;
			}
			else
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
		}
		
		void add(const T& obj)
		{
			objects.push_back(obj);
		}
		
		void add(T&& obj)
		{
			objects.push_back(obj);
		}
		
		void add(size_t index, const T& obj)
		{
			if(index <= objects.size())
			{
				objects.insert(objects.begin()+index, obj);
				return;
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		void add(size_t index, T&& obj)
		{
			if(index <= objects.size())
			{
				objects.insert(objects.begin()+index, obj);
				return;
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		void remove(size_t index)
		{
			if(index < objects.size())
			{
				objects.erase(objects.begin()+index);
				return;
			}
			#ifndef _ARRAYLIST_STANDALONE
				throw ArrayListOutOfBoundsException(index, objects.size());
			#else
				throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
			#endif
		}
		
		void clear()
		{
			objects.clear();
		}
		
		size_t size() const
		{
			return objects.size();
		}

		void resize(size_t size)
		{
			objects.resize(size);
		}
		
		void reserve(size_t size)
		{
			objects.reserve(size);
		}
		
		void shrinkToFit()
		{
			objects.shrink_to_fit();
		}
		
		size_t indexOf(const T& obj) const
		{
			size_t length = objects.size();
			for(size_t i=0; i<length; i++)
			{
				const T& cmp = (const T&)objects[i];
				if(obj==cmp)
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		
		bool contains(const T& obj) const
		{
			return indexOf(obj) != ArrayList<T>::NOT_FOUND;
		}
		
		String toString() const
		{
			String str = "[ ";
			size_t length = objects.size();
			size_t lastIndex = length-1;
			for(size_t i=0; i<length; i++)
			{
				str += fgl::stringify<T>(objects[i]);
				if(i != lastIndex)
				{
					str += ", ";
				}
			}
			str += " ]";
			return str;
		}
	};
	
	template<typename ARRAYLIST_TYPE, typename VALUE_TYPE = typename ARRAYLIST_TYPE::value_type,
		bool CHECK = std::is_same<ARRAYLIST_TYPE, ArrayList<VALUE_TYPE> >::value>
	struct is_ArrayList
	{
		static constexpr bool value = false;
	};
	
	template<typename ARRAYLIST_TYPE, typename VALUE_TYPE>
	struct is_ArrayList<ARRAYLIST_TYPE, VALUE_TYPE, true>
	{
		static constexpr bool value = true;
		typedef VALUE_TYPE value_type;
		typedef ArrayList<VALUE_TYPE> arraylist_type;
		typedef std::nullptr_t null_type;
	};
	
#ifndef _ARRAYLIST_STANDALONE
}
#endif

