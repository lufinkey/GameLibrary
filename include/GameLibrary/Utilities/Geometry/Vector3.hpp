
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include <GameLibrary/Utilities/Math.hpp>
#include "Vector2.hpp"

namespace fgl
{
	template<typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;
		
		constexpr Vector3() : x(0), y(0), z(0) {
			//
		}
		
		constexpr Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {
			//
		}
		
		constexpr Vector3(const Vector2<T>& vect2, T z) : x(vect2.x), y(vect2.y), z(vect2.z) {
			//
		}
		
		constexpr Vector3(const std::tuple<T,T,T>& tuple) : x(std::get<0>(tuple)), y(std::get<1>(tuple)), z(std::get<2>(tuple)) {
			//
		}
		
		template<typename U>
		constexpr explicit Vector3(const Vector3<U>& vect) : x(static_cast<T>(vect.x)), y(static_cast<T>(vect.y)), z(static_cast<T>(vect.z)) {
			//
		}

		template<typename _T=T, typename std::enable_if<std::is_signed<_T>::value, std::nullptr_t>::type = nullptr>
		constexpr Vector3<T> operator-() const {
			return Vector3<T>(-x, -y, -z);
		}
		
		constexpr Vector3<T>& operator+=(const Vector3<T>& vect) {
			x += vect.x;
			y += vect.y;
			z += vect.z;
			return *this;
		}
		
		constexpr Vector3<T>& operator-=(const Vector3<T>& vect) {
			x -= vect.x;
			y -= vect.y;
			z -= vect.z;
			return *this;
		}
		
		constexpr Vector3<T>& operator*=(const Vector3<T>& vect) {
			x *= vect.x;
			y *= vect.y;
			z *= vect.z;
			return *this;
		}
		
		constexpr Vector3<T>& operator/=(const Vector3<T>& vect) {
			x /= vect.x;
			y /= vect.y;
			z /= vect.z;
			return *this;
		}
		
		constexpr Vector3<T>& operator+=(const std::tuple<T,T,T>& tuple) {
			x += std::get<0>(tuple);
			y += std::get<1>(tuple);
			z += std::get<2>(tuple);
			return *this;
		}
		
		constexpr Vector3<T>& operator-=(const std::tuple<T,T,T>& tuple) {
			x -= std::get<0>(tuple);
			y -= std::get<1>(tuple);
			z -= std::get<2>(tuple);
			return *this;
		}
		
		constexpr Vector3<T>& operator*=(const std::tuple<T,T,T>& tuple) {
			x *= std::get<0>(tuple);
			y *= std::get<1>(tuple);
			z *= std::get<2>(tuple);
			return *this;
		}
		
		constexpr Vector3<T>& operator/=(const std::tuple<T,T,T>& tuple) {
			x /= std::get<0>(tuple);
			y /= std::get<1>(tuple);
			z /= std::get<2>(tuple);
			return *this;
		}
		
		constexpr Vector3<T>& operator*=(const T& num) {
			x *= num;
			y *= num;
			z *= num;
			return *this;
		}
		
		constexpr Vector3<T>& operator/=(const T& num) {
			x /= num;
			y /= num;
			z /= num;
			return *this;
		}
		
		constexpr bool operator==(const Vector3<T>& vect) const {
			return equals(vect);
		}
		
		constexpr bool operator!=(const Vector3<T>& vect) const {
			return !equals(vect);
		}
		
		constexpr bool equals(const Vector3<T>& vect) const {
			if (x == vect.x && y == vect.y && z == vect.z) {
				return true;
			}
			return false;
		}
		
		constexpr Vector3<T> plus(const Vector3<T>& vect) const {
			return Vector3<T>(x+vect.x, y+vect.y, z+vect.z);
		}
		
		constexpr Vector3<T> minus(const Vector3<T>& vect) const {
			return Vector3<T>(x-vect.x, y-vect.y, z-vect.z);
		}
		
		constexpr Vector3<T> multiplied(const Vector3<T>& vect) const {
			return Vector3<T>(x*vect.x, y*vect.y, z*vect.z);
		}
		
		constexpr Vector3<T> divided(const Vector3<T>& vect) const {
			return Vector3<T>(x/vect.x, y/vect.y, z/vect.z);
		}
		
		template<typename _T=T, typename std::enable_if<std::is_floating_point<_T>::value, std::nullptr_t>::type = nullptr>
		constexpr Vector3<T> mod(const Vector3<T>& vect) const {
			return Vector3<T>(Math::fmod(x, vect.x), Math::fmod(y, vect.y), Math::fmod(z, vect.z));
		}
		
		template<typename _T=T, typename std::enable_if<!std::is_floating_point<_T>::value, std::nullptr_t>::type = nullptr>
		constexpr Vector3<T> mod(const Vector3<T>& vect) const {
			return Vector3<T>(x % vect.x, y % vect.y, z % vect.z);
		}
		
		constexpr Vector2<T> getVector2() const {
			return Vector2<T>(x, y);
		}
		
		String toString() const {
			return "Vector3(x:" + fgl::stringify<T>(x) + ", y:" + fgl::stringify<T>(y) + ", z:" + fgl::stringify<T>(z) + ")";
		}
	};
	
	
	
	template<typename T>
	constexpr Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right) {
		return Vector3<T>(left.x+right.x, left.y+right.y, left.z+right.z);
	}
	
	template<typename T>
	constexpr Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right) {
		return Vector3<T>(left.x-right.x, left.y-right.y, left.z-right.z);
	}
	
	template<typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right) {
		return Vector3<T>(left.x*right.x, left.y*right.y, left.z*right.z);
	}
	
	template<typename T>
	constexpr Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right) {
		return Vector3<T>(left.x/right.x, left.y/right.y, left.z/right.z);
	}
	
	template<typename T>
	constexpr Vector3<T> operator+(const Vector3<T>& left, const std::tuple<T,T,T>& right) {
		return Vector3<T>(left.x+std::get<0>(right), left.y+std::get<1>(right), left.z+std::get<2>(right));
	}
	
	template<typename T>
	constexpr Vector3<T> operator-(const Vector3<T>& left, const std::tuple<T,T,T>& right) {
		return Vector3<T>(left.x-std::get<0>(right), left.y-std::get<1>(right), left.z-std::get<2>(right));
	}
	
	template<typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& left, const std::tuple<T,T,T>& right) {
		return Vector3<T>(left.x*std::get<0>(right), left.y*std::get<1>(right), left.z*std::get<2>(right));
	}
	
	template<typename T>
	constexpr Vector3<T> operator/(const Vector3<T>& left, const std::tuple<T,T,T>& right) {
		return Vector3<T>(left.x/std::get<0>(right), left.y/std::get<1>(right), left.z/std::get<2>(right));
	}
	
	template<typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& left, const T& right) {
		return Vector3<T>(left.x*right, left.y*right, left.z*right);
	}
	
	template<typename T>
	constexpr Vector3<T> operator/(const Vector3<T>& left, const T& right) {
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
	typedef Vector3<size_t> Vector3s;
}
