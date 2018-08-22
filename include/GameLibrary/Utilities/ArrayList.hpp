
#pragma once

#include <array>
#include <algorithm>
#include <climits>
#include <initializer_list>
#include <type_traits>
#include <vector>
#include <list>
#include <functional>
#include <iterator>

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

#ifdef ARRAYLIST_STANDALONE
#ifndef ARRAYLIST_DEFAULT_PREALLOC_COUNT
#define ARRAYLIST_DEFAULT_PREALLOC_COUNT 0
#endif
#endif

#ifdef ARRAYLIST_STANDALONE
#include <stdexcept>
#else
#include <GameLibrary/Types.hpp>
#include <GameLibrary/Exception/Utilities/ArrayListOutOfBoundsException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>
#include "Stringifier.hpp"

namespace fgl
{
#endif
	//Mostly just a wrapper around std::vector
	#ifdef ARRAYLIST_STANDALONE
	template <typename T, size_t PREALLOC_COUNT = ARRAYLIST_DEFAULT_PREALLOC_COUNT>
	#else
	template <typename T, size_t PREALLOC_COUNT>
	#endif
	class ArrayList
	{
		template<typename InputIterator>
		using RequireInputIterator = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type;
		
	private:
		std::vector<T> objects;
		
		void updatePreallocation(size_t objects_size)
		{
			size_t prealloc_count = PREALLOC_COUNT;
			if(prealloc_count > 0)
			{
				size_t numBlocks = objects_size / prealloc_count;
				if((objects_size % prealloc_count) != 0)
				{
					numBlocks++;
				}
				objects.reserve(numBlocks*prealloc_count);
			}
			else
			{
				objects.reserve(objects_size);
			}
		}
		
	public:
		static constexpr size_t NOT_FOUND = (size_t)-1;
		typedef T value_type;
		
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::reverse_iterator reverse_iterator;
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
		
		template<typename _T, size_t _PREALLOC_COUNT>
		friend class ArrayList;
		
		ArrayList()
		{
			//
		}
		
		template<size_t _PREALLOC_COUNT>
		ArrayList(const ArrayList<T, _PREALLOC_COUNT>& array)
			: objects(array.objects)
		{
			//
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<(!std::is_same<U,_T>::value && std::is_convertible<U,_T>::value), std::nullptr_t>::type = nullptr>
		explicit ArrayList(const ArrayList<U, _PREALLOC_COUNT>& array)
			: objects(array.objects.begin(), array.objects.end())
		{
			//
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<(!std::is_same<U,_T>::value && std::is_convertible<U,_T>::value), std::nullptr_t>::type = nullptr>
		explicit ArrayList(ArrayList<U, _PREALLOC_COUNT>&& array)
			: objects(std::make_move_iterator(array.objects.begin()), std::make_move_iterator(array.objects.end()))
		{
			//
		}
		
		template<size_t _PREALLOC_COUNT>
		ArrayList(ArrayList<T, _PREALLOC_COUNT>&& array)
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
		
		template<size_t SIZE>
		ArrayList(std::array<T, SIZE>&& arr)
			: objects(std::make_move_iterator(arr.begin()), std::make_move_iterator(arr.end()))
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
		
		ArrayList(const std::list<T>& list)
			: objects(list.begin(), list.end())
		{
			//
		}
		
		ArrayList(std::list<T>&& list)
			: objects(std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()))
		{
			//
		}
		
		ArrayList(std::initializer_list<T> list)
			: objects(list)
		{
			//
		}
		
		template<typename InputIterator, typename = RequireInputIterator<InputIterator>>
		ArrayList(InputIterator beginIt, InputIterator endIt)
			: objects(beginIt, endIt)
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
		
		template<size_t _PREALLOC_COUNT>
		ArrayList<T, PREALLOC_COUNT>& operator=(const ArrayList<T, _PREALLOC_COUNT>& array)
		{
			objects = array.objects;
			return *this;
		}
		
		template<size_t _PREALLOC_COUNT>
		ArrayList<T, PREALLOC_COUNT>& operator=(ArrayList<T, _PREALLOC_COUNT>&& array)
		{
			objects = array.objects;
			return *this;
		}
		
		ArrayList<T, PREALLOC_COUNT>& operator=(std::initializer_list<T> list)
		{
			objects = list;
			return *this;
		}
		
		T& operator[] (size_t index)
		{
			if(index >= objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			objects[index] = obj;
		}
		
		void add(const T& obj)
		{
			updatePreallocation(objects.size()+1);
			objects.push_back(obj);
		}
		
		void add(T&& obj)
		{
			updatePreallocation(objects.size()+1);
			objects.push_back(obj);
		}
		
		void add(size_t index, const T& obj)
		{
			if(index > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			updatePreallocation(objects.size()+1);
			objects.insert(objects.begin()+index, obj);
		}
		
		void add(size_t index, T&& obj)
		{
			if(index > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			updatePreallocation(objects.size()+1);
			objects.insert(objects.begin()+index, obj);
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<std::is_convertible<U, _T>::value, std::nullptr_t>::type = nullptr>
		void addAll(const ArrayList<U, _PREALLOC_COUNT>& array)
		{
			updatePreallocation(objects.size()+array.size());
			objects.insert(objects.end(), array.begin(), array.end());
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<std::is_convertible<U, _T>::value, std::nullptr_t>::type = nullptr>
		void addAll(ArrayList<U, _PREALLOC_COUNT>&& array)
		{
			updatePreallocation(objects.size()+array.size());
			objects.insert(objects.end(), std::make_move_iterator(array.begin()), std::make_move_iterator(array.end()));
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<std::is_convertible<U, _T>::value, std::nullptr_t>::type = nullptr>
		void addAll(size_t index, const ArrayList<U, _PREALLOC_COUNT>& array)
		{
			if(index > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			updatePreallocation(objects.size()+array.size());
			objects.insert(objects.begin()+index, array.begin(), array.end());
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<std::is_convertible<U, _T>::value, std::nullptr_t>::type = nullptr>
		void addAll(size_t index, ArrayList<U, _PREALLOC_COUNT>&& array)
		{
			if(index > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(index, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			updatePreallocation(objects.size()+array.size());
			objects.insert(objects.begin()+index, std::make_move_iterator(array.begin()), std::make_move_iterator(array.end()));
		}
		
		template<typename InputIterator, typename = RequireInputIterator<InputIterator>>
		void add(InputIterator beginIt, InputIterator endIt) {
			objects.insert(objects.end(), beginIt, endIt);
		}
		
		template<typename InputIterator, typename = RequireInputIterator<InputIterator>>
		void add(size_t index, InputIterator beginIt, InputIterator endIt) {
			objects.insert(objects.begin()+index, beginIt, endIt);
		}
		
		void remove(size_t index)
		{
			if(index >= objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(startIndex, objects.size());
				#else
					throw std::out_of_range("startIndex " + std::to_string(startIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			if(endIndex > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
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
				#ifndef ARRAYLIST_STANDALONE
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
		
		size_t capacity() const
		{
			return objects.capacity();
		}
		
		void resize(size_t size)
		{
			objects.resize(size);
		}
		
		void resize(size_t size, const T& value)
		{
			objects.resize(size, value);
		}
		
		void reserve(size_t size)
		{
			objects.reserve(size);
		}
		
		void shrinkToFit()
		{
			objects.shrink_to_fit();
		}
		
		template<typename U, size_t _PREALLOC_COUNT,
			typename _T=T,
			typename std::enable_if<std::is_convertible<U, _T>::value, std::nullptr_t>::type = nullptr>
		ArrayList<T, PREALLOC_COUNT> mergedWith(const ArrayList<U, _PREALLOC_COUNT>& arr) const
		{
			ArrayList<T, PREALLOC_COUNT> newArr;
			newArr.reserve(objects.size() + arr.size());
			newArr.addAll(*this);
			newArr.addAll(arr);
			return newArr;
		}
		
		T& firstWhere(const std::function<bool(const T&)>& func) {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			throw fgl::ArrayListOutOfBoundsException(objects.size(), objects.size());
		}
		
		const T& firstWhere(const std::function<bool(const T&)>& func) const {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			throw fgl::ArrayListOutOfBoundsException(objects.size(), objects.size());
		}
		
		T& firstWhere(const std::function<bool(const T&)>& func, T& defaultValue) {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			return defaultValue;
		}
		
		const T& firstWhere(const std::function<bool(const T&)>& func, const T& defaultValue) const {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			return defaultValue;
		}
		
		#ifdef __OBJC__
		T& firstWhere(BOOL(^func)(const T&)) {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			throw fgl::ArrayListOutOfBoundsException(objects.size(), objects.size());
		}
		
		const T& firstWhere(BOOL(^func)(const T&)) const {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			throw fgl::ArrayListOutOfBoundsException(objects.size(), objects.size());
		}
		
		T& firstWhere(BOOL(^func)(const T&), T& defaultValue) {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			return defaultValue;
		}
		
		const T& firstWhere(BOOL(^func)(const T&), const T& defaultValue) const {
			for(auto& object : objects) {
				if(func(object)) {
					return object;
				}
			}
			return defaultValue;
		}
		#endif
		
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
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
			return ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
		}
		#endif
		
		bool contains(const T& obj) const
		{
			return indexOf(obj) != ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
		}

		bool containsWhere(const std::function<bool(const T&)>& func) const
		{
			return indexWhere(func) != ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
		}

		#ifdef __OBJC__
		bool containsWhere(BOOL(^func)(const T&)) const
		{
			return indexWhere(func) != ArrayList<T, PREALLOC_COUNT>::NOT_FOUND;
		}
		#endif
		
		ArrayList<T, PREALLOC_COUNT> filter(const std::function<bool(const T&)>& func) const
		{
			ArrayList<T, PREALLOC_COUNT> newList;
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
		ArrayList<T, PREALLOC_COUNT> filter(BOOL(^func)(const T&)) const
		{
			ArrayList<T, PREALLOC_COUNT> newList;
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
			if(objects.size() <= 1)
			{
				return;
			}
			std::stable_sort(objects.begin(), objects.end(), [&](const T& a, const T& b) -> bool {
				return func(a, b);
			});
		}
		
		#ifdef __OBJC__
		void sort(BOOL(^func)(const T&,const T&))
		{
			if(objects.size() <= 1)
			{
				return;
			}
			std::stable_sort(objects.begin(), objects.end(), [&](const T& obj1, const T& obj2) -> bool {
				return func(obj1, obj2);
			});
		}
		#endif
		
		template<typename U, size_t _PREALLOC_COUNT=PREALLOC_COUNT>
		ArrayList<U, _PREALLOC_COUNT> map(const std::function<U(const T& value, size_t index)>& func) const
		{
			ArrayList<U, _PREALLOC_COUNT> mappedArray;
			size_t objects_size = objects.size();
			mappedArray.reserve(objects_size);
			for(size_t i=0; i<objects_size; i++)
			{
				mappedArray.add(func(objects[i], i));
			}
			return mappedArray;
		}
		
		#ifdef __OBJC__
		template<typename U, size_t _PREALLOC_COUNT=PREALLOC_COUNT>
		ArrayList<U, _PREALLOC_COUNT> map(U(^func)(const T& value, size_t index)) const
		{
			return map([&](const T& value, size_t index) {
				return func(value, index);
			});
		}
		#endif
		
		ArrayList<T, PREALLOC_COUNT> unique() const
		{
			ArrayList<T, PREALLOC_COUNT> newList = *this;
			newList.removeDuplicates();
			return std::move(newList);
		}
		
		ArrayList<T, PREALLOC_COUNT> subArray(size_t startIndex, size_t endIndex) const
		{
			if(startIndex > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(startIndex, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			else if(endIndex > objects.size())
			{
				#ifndef ARRAYLIST_STANDALONE
					throw ArrayListOutOfBoundsException(endIndex, objects.size());
				#else
					throw std::out_of_range("index " + std::to_string(endIndex) + " is out of bounds in ArrayList with a size of " + std::to_string(objects.size()));
				#endif
			}
			return ArrayList<T, PREALLOC_COUNT>(std::vector<T>(objects.begin()+startIndex, objects.begin()+endIndex));
		}
		
		ArrayList<T, PREALLOC_COUNT> subArray(size_t startIndex) const
		{
			return subArray(startIndex, objects.size());
		}
		
		ArrayList<T, PREALLOC_COUNT> reversed() const
		{
			return ArrayList<T, PREALLOC_COUNT>(std::vector<T>(objects.rbegin(), objects.rend()));
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
	
#ifndef ARRAYLIST_STANDALONE
}
#endif

