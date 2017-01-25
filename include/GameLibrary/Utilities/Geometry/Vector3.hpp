
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>

namespace fgl
{
	template<typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;
		
		Vector3() : x(0), y(0), z(0)
		{
			//
		}
		
		Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z)
		{
			//
		}
		
		template<typename U>
		explicit Vector3(const Vector3<U>& vect) : x(static_cast<T>(vect.x)), y(static_cast<T>(vect.y)), z(static_cast<T>(vect.z))
		{
			//
		}
		
		Vector3<T>& operator+=(const Vector3<T>& vect)
		{
			x+=vect.x;
			y+=vect.y;
			z+=vect.z;
			return *this;
		}
		
		Vector3<T>& operator-=(const Vector3<T>& vect)
		{
			x-=vect.x;
			y-=vect.y;
			z-=vect.z;
			return *this;
		}
		
		Vector3<T>& operator*=(const Vector3<T>& vect)
		{
			x*=vect.x;
			y*=vect.y;
			z*=vect.z;
			return *this;
		}
		
		Vector3<T>& operator/=(const Vector3<T>& vect)
		{
			x/=vect.x;
			y/=vect.y;
			z/=vect.z;
			return *this;
		}
		
		Vector3<T>& operator*=(const T& num)
		{
			x*=num;
			y*=num;
			z*=num;
			return *this;
		}
		
		Vector3<T>& operator/=(const T& num)
		{
			x/=num;
			y/=num;
			z/=num;
			return *this;
		}
		
		bool operator==(const Vector3<T>& vect) const
		{
			if (x == vect.x && y == vect.y && z == vect.z)
			{
				return true;
			}
			return false;
		}
		
		bool operator!=(const Vector3<T>& vect) const
		{
			if (x == vect.x && y == vect.y && z == vect.z)
			{
				return false;
			}
			return true;
		}
		
		bool equals(const Vector3<T>& vect) const
		{
			if (x == vect.x && y == vect.y && z == vect.z)
			{
				return true;
			}
			return false;
		}
		
		String toString() const
		{
			return "Vector3(x:" + fgl::stringify<T>(x) + ", y:" + fgl::stringify<T>(y) + ", z:" + fgl::stringify<T>(z) + ")";
		}
	};
	
	template<typename T>
	Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right)
	{
		return Vector3<T>(left.x+right.x, left.y+right.y, left.z+right.z);
	}
	
	template<typename T>
	Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right)
	{
		return Vector3<T>(left.x-right.x, left.y-right.y, left.z-right.z);
	}
	
	template<typename T>
	Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right)
	{
		return Vector3<T>(left.x*right.x, left.y*right.y, left.z*right.z);
	}
	
	template<typename T>
	Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right)
	{
		return Vector3<T>(left.x/right.x, left.y/right.y, left.z/right.z);
	}
	
	template<typename T>
	Vector3<T> operator*(const Vector3<T>& left, const T& right)
	{
		return Vector3<T>(left.x*right, left.y*right, left.z*right);
	}
	
	template<typename T>
	Vector3<T> operator/(const Vector3<T>& left, const T& right)
	{
		return Vector3<T>(left.x/right, left.y/right, left.z/right);
	}
	
	
	
	typedef Vector3<byte> Vector3b;
	typedef Vector3<int> Vector3i;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	typedef Vector3<unsigned int> Vector3u;
	typedef Vector3<long> Vector3l;
	typedef Vector3<long long> Vector3ll;
	typedef Vector3<unsigned long long> Vector3ull;
	typedef Vector3<long double> Vector3ld;
}
