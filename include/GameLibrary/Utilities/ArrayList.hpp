
#pragma once

#include <array>
#include <algorithm>
#include <climits>
#include <initializer_list>
#include <type_traits>
#include <vector>
#include <functional>

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

#ifdef _ARRAYLIST_STANDALONE
#include <stdexcept>
#else
#include <GameLibrary/Exception/Utilities/ArrayListOutOfBoundsException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>
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
		
		ArrayList(const ArrayList<T>& array)
			: objects(array.objects)
		{
			//
		}
		
		ArrayList(ArrayList<T>&& array)
			: objects(array.objects)
		{
			//
		}
		
		template<size_t SIZE>
		ArrayList(const std::array<T, SIZE>& arr)
			: objects(arr.begin(), arr.end())
		{
			//
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
		
		ArrayList<T>& operator=(const ArrayList<T>& array)
		{
			objects = array.objects;
			return *this;
		}
		
		ArrayList<T>& operator=(ArrayList<T>&& array)
		{
			objects = array.objects;
			return *this;
		}
		
		ArrayList<T>& operator=(std::initializer_list<T> list)
		{
			objects = list;
			return *this;
		}
		
		T& operator[] (size_t index)
		{
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return objects[index];
		}
		
		const T& operator[] (size_t index) const
		{
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return objects[index];
		}
		
		T& get(size_t index)
		{
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return objects[index];
		}
		
		const T& get(size_t index) const
		{
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return objects[index];
		}

		const T* getData() const
		{
			return objects.data();
		}

		std::vector<T>& getVector()
		{
			return objects;
		}

		const std::vector<T>& getVector() const
		{
			return objects;
		}
		
		void set(size_t index, const T& obj)
		{
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects[index] = obj;
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
			objects.insert(objects.end(), array.objects.begin(), array.objects.end());
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
			objects.insert(objects.begin()+index, array.objects.begin(), array.objects.end());
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
			if(index >= objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects.erase(objects.begin()+index);
		}

		void remove(size_t startIndex, size_t endIndex)
		{
			if(startIndex > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(startIndex, objects.size());
				#else
					throw std::out_of_range("startIndex " + std::to_string(startIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			if(endIndex > objects.size())
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(endIndex, objects.size());
				#else
					throw std::out_of_range("endIndex " + std::to_string(endIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects.erase(objects.begin()+startIndex, objects.begin()+endIndex);
		}

		size_t removeEqual(const T& cmp)
		{
			size_t removeCount = 0;
			size_t firstRemoveIndex = -1;
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				if(objects[i]==cmp)
				{
					removeCount++;
					if(firstRemoveIndex==-1)
					{
						firstRemoveIndex = i;
					}
				}
				else
				{
					if(firstRemoveIndex!=-1)
					{
						objects.erase(objects.begin()+i+1, objects.begin()+firstRemoveIndex+1);
						firstRemoveIndex = -1;
					}
				}
			}
			if(firstRemoveIndex!=-1)
			{
				objects.erase(objects.begin(), objects.begin()+firstRemoveIndex+1);
			}
			return removeCount;
		}

		bool removeFirstEqual(const T& cmp)
		{
			for(size_t objects_size=objects.size(), i=0; i<objects_size; i++)
			{
				const T& obj = objects[i];
				if(obj==cmp)
				{
					objects.erase(objects.begin()+i);
					return true;
				}
			}
			return false;
		}

		bool removeLastEqual(const T& cmp)
		{
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				const T& obj = objects[i];
				if(obj==cmp)
				{
					objects.erase(objects.begin()+i);
					return true;
				}
			}
			return false;
		}

		size_t removeWhere(const std::function<bool(const T&)>& func)
		{
			size_t removeCount = 0;
			size_t firstRemoveIndex = -1;
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				if(func(objects[i]))
				{
					removeCount++;
					if(firstRemoveIndex==-1)
					{
						firstRemoveIndex = i;
					}
				}
				else
				{
					if(firstRemoveIndex!=-1)
					{
						objects.erase(objects.begin()+i+1, objects.begin()+firstRemoveIndex+1);
						firstRemoveIndex = -1;
					}
				}
			}
			if(firstRemoveIndex!=-1)
			{
				objects.erase(objects.begin(), objects.begin()+firstRemoveIndex+1);
			}
			return removeCount;
		}

		bool removeFirstWhere(const std::function<bool(const T&)>& func)
		{
			for(size_t objects_size=objects.size(), i=0; i<objects_size; i++)
			{
				const T& obj = objects[i];
				if(func(obj))
				{
					objects.erase(objects.begin()+i);
					return true;
				}
			}
			return false;
		}

		bool removeLastWhere(const std::function<bool(const T&)>& func)
		{
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				const T& obj = objects[i];
				if(func(obj))
				{
					objects.erase(objects.begin()+i);
					return true;
				}
			}
			return false;
		}

		void removeDuplicates()
		{
			for(size_t i=0; i<objects.size(); i++)
			{
				auto& obj = objects[i];
				for(size_t j=(objects.size()-1); j!=i; j--)
				{
					if(obj==objects[j])
					{
						objects.erase(objects.begin()+j);
					}
				}
			}
		}

		T pop()
		{
			if(objects.size()==0)
			{
				#ifndef _ARRAYLIST_STANDALONE
					throw IllegalStateException("cannot call \"pop\" when ArrayList is empty");
				#else
					throw std::logic_error("cannot call \"pop\" when ArrayList is empty");
				#endif
			}
			T t = objects[objects.size()-1];
			objects.erase(objects.begin()+(objects.size()-1));
			return t;
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
			for(size_t objects_size=objects.size(), i=0; i<objects_size; i++)
			{
				const T& cmp = objects[i];
				if(obj==cmp)
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		
		size_t indexWhere(const std::function<bool(const T&)>& func) const
		{
			for(size_t objects_size=objects.size(), i=0; i<objects_size; i++)
			{
				if(func(objects[i]))
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		
		#ifdef __OBJC__
		size_t indexWhere(BOOL(^func)(const T&)) const
		{
			for(size_t objects_size=objects.size(), i=0; i<objects_size; i++)
			{
				if(func(objects[i]))
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		#endif
		
		size_t lastIndexOf(const T& obj) const
		{
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				const T& cmp = objects[i];
				if(obj==cmp)
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		
		size_t lastIndexWhere(const std::function<bool(const T&)>& func) const
		{
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				if(func(objects[i]))
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		
		#ifdef __OBJC__
		size_t lastIndexWhere(BOOL(^func)(const T&)) const
		{
			for(size_t i=(objects.size()-1); i!=-1; i--)
			{
				if(func(objects[i]))
				{
					return i;
				}
			}
			return ArrayList<T>::NOT_FOUND;
		}
		#endif
		
		bool contains(const T& obj) const
		{
			return indexOf(obj) != ArrayList<T>::NOT_FOUND;
		}
		
		ArrayList<T> filter(const std::function<bool(const T&)>& func) const
		{
			ArrayList<T> newList;
			size_t length = objects.size();
			newList.reserve(length);
			for(size_t i=0; i<length; i++)
			{
				const T& obj = objects[i];
				if(func(obj))
				{
					newList.add(obj);
				}
			}
			newList.shrinkToFit();
			return newList;
		}
		
		#ifdef __OBJC__
		ArrayList<T> filter(BOOL(^func)(const T&)) const
		{
			ArrayList<T> newList;
			size_t length = objects.size();
			newList.reserve(length);
			for(size_t i=0; i<length; i++)
			{
				const T& obj = objects[i];
				if(func(obj))
				{
					newList.add(obj);
				}
			}
			newList.shrinkToFit();
			return newList;
		}
		#endif
		
		void sort(const std::function<bool(const T&,const T&)>& func)
		{
			if(objects.size()<=1)
			{
				return;
			}
			std::stable_sort(objects.begin(), objects.end(), func);
		}
		
		#ifdef __OBJC__
		void sort(BOOL(^func)(const T&,const T&))
		{
			if(objects.size()<=1)
			{
				return;
			}
			std::stable_sort(objects.begin(), objects.end(), [&](const T& obj1, const T& obj2) -> bool {
				return func(obj1, obj2);
			});
		}
		#endif

		ArrayList<T> unique() const
		{
			ArrayList<T> newList = *this;
			newList.removeDuplicates();
			return std::move(newList);
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
		
		ArrayList<T> reversed() const
		{
			return ArrayList<T>(std::vector<T>(objects.rbegin(), objects.rend()));
		}
		
		void forEach(const std::function<void(T&)>& func)
		{
			for(auto& obj : objects)
			{
				func(obj);
			}
		}
		
		void forEach(const std::function<void(const T&)>& func) const
		{
			for(auto& obj : objects)
			{
				func(obj);
			}
		}
		
		#ifdef __OBJC__
		void forEach(void(^func)(T&))
		{
			for(auto& obj : objects)
			{
				func(obj);
			}
		}
		
		void forEach(void(^func)(const T&)) const
		{
			for(auto& obj : objects)
			{
				func(obj);
			}
		}
		#endif
		
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

