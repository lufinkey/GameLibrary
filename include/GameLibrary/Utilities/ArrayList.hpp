
#pragma once

#include <array>
#include <algorithm>
#include <climits>
#include <initializer_list>
#include <type_traits>
#include <vector>
#include <functional>

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
		
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::reverse_iterator reverse_iterator;
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
		
		ArrayList()
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
		
		ArrayList(std::vector<T>&& vect)
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
		
		iterator begin()
		{
			return objects.begin();
		}
		
		const_iterator begin() const
		{
			return objects.begin();
		}
		
		const_iterator cbegin() const
		{
			return objects.cbegin();
		}
		
		iterator end()
		{
			return objects.end();
		}
		
		const_iterator end() const
		{
			return objects.end();
		}
		
		const_iterator cend() const
		{
			return objects.cend();
		}
		
		reverse_iterator rbegin()
		{
			return objects.rbegin();
		}
		
		const_reverse_iterator rbegin() const
		{
			return objects.rbegin();
		}
		
		const_reverse_iterator crbegin() const
		{
			return objects.crbegin();
		}
		
		reverse_iterator rend()
		{
			return objects.rend();
		}
		
		const_reverse_iterator rend() const
		{
			return objects.rend();
		}
		
		const_reverse_iterator crend() const
		{
			return objects.crend();
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
			if(index > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects.insert(objects.begin()+index, obj);
		}
		
		void add(size_t index, T&& obj)
		{
			if(index > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects.insert(objects.begin()+index, obj);
		}
		
		void addAll(const ArrayList<T>& array)
		{
			objects.reserve(objects.size()+array.objects.size());
			for(size_t array_size=array.objects.size(), i=0; i<array_size; i++)
			{
				objects.push_back(array.objects[i]);
			}
		}
		
		void addAll(ArrayList<T>&& array)
		{
			objects.reserve(objects.size()+array.objects.size());
			for(size_t array_size=array.objects.size(), i=0; i<array_size; i++)
			{
				objects.push_back(std::move(array.objects[i]));
			}
		}
		
		void addAll(size_t index, const ArrayList<T>& array)
		{
			size_t size = objects.size();
			if(index > size)
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			size_t array_size = array.objects.size();
			if(array_size > 0)
			{
				size_t fromIndex = index;
				if(fromIndex == size)
				{
					objects.reserve(size+array_size);
					for(size_t i=0; i<array_size; i++)
					{
						objects.push_back(array.objects[i]);
					}
				}
				else
				{
					size_t toIndex = index + array.objects.size();
					objects.resize(size+array_size);
					while (toIndex < size)
					{
						objects[toIndex] = objects[fromIndex];
						fromIndex++;
						toIndex++;
					}
					size_t counter = index;
					for(size_t i=0; i<array_size; i++)
					{
						objects[counter] = array.objects[i];
						counter++;
					}
				}
			}
		}
		
		void addAll(size_t index, ArrayList<T>&& array)
		{
			size_t size = objects.size();
			if(index > size)
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			size_t array_size = array.objects.size();
			if(array_size > 0)
			{
				size_t fromIndex = index;
				if(fromIndex == size)
				{
					objects.reserve(size+array_size);
					for(size_t i=0; i<array_size; i++)
					{
						objects.push_back(std::move(array.objects[i]));
					}
				}
				else
				{
					size_t toIndex = index + array.objects.size();
					objects.resize(size+array_size);
					while (toIndex < size)
					{
						objects[toIndex] = objects[fromIndex];
						fromIndex++;
						toIndex++;
					}
					size_t counter = index;
					for(size_t i=0; i<array_size; i++)
					{
						objects[counter] = std::move(array.objects[i]);
						counter++;
					}
				}
			}
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
		
		ArrayList<T> filter(const std::function<bool(const T&)>& func) const
		{
			ArrayList<T> newList;
			size_t length = objects.size();
			for(size_t i=0; i<length; i++)
			{
				const T& obj = objects[i];
				if(func(obj))
				{
					newList.add(obj);
				}
			}
			return newList;
		}
		
		void sort(const std::function<bool(const T&,const T&)>& func)
		{
			if(objects.size()<=1)
			{
				return;
			}
			std::stable_sort(objects.begin(), objects.end(), func);
		}
		
		ArrayList<T> subArray(size_t startIndex, size_t endIndex) const
		{
			if(startIndex > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(startIndex, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			else if(endIndex > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(endIndex, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(endIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return ArrayList<T>(std::vector<T>(objects.begin()+startIndex, objects.begin()+endIndex));
		}
		
		ArrayList<T> subArray(size_t startIndex) const
		{
			return subArray(startIndex, objects.size());
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

