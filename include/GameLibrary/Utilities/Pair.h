
#pragma once

#include "Stringifier.h"

namespace fgl
{
	template<class T1, class T2>
	class Pair
	{
	public:
		T1 first;
		T2 second;
		
		typedef T1 first_type;
		typedef T2 second_type;
		typedef Pair<T1, T2> pair_type;
		
		Pair() : first(), second()
		{
			//
		}
		
		Pair(T1&& v1, T2&& v2) : first(v1), second(v2)
		{
			//
		}
		
		Pair(const T1& v1, const T2& v2) : first(v1), second(v2)
		{
			//
		}
		
		Pair(pair_type&& p) : first(p.first), second(p.second)
		{
			//
		}
		
		Pair(const pair_type& p) : first(p.first), second(p.second)
		{
			//
		}
		
		template<typename U1, typename U2>
		explicit Pair(const Pair<U1, U2>& p)
			: first((T1)p.first), second((T2)p.second)
		{
			//
		}
		
		pair_type& operator=(pair_type&& p)
		{
			first = p.first;
			second = p.second;
			return *this;
		}
		
		pair_type& operator=(const pair_type& p)
		{
			first = p.first;
			second = p.second;
			return *this;
		}
		
		template<class U1, class U2>
		pair_type& operator=(const Pair<U1, U2>& p)
		{
			first = (T1)p.first;
			second = (T2)p.second;
			return *this;
		}
		
		String toString() const
		{
			return "Pair( " + fgl::stringify<T1>(first) + ", " + fgl::stringify<T2>(second) + " )";
		}
	};
}