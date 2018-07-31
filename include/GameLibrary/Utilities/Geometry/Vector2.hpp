
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Stringifier.hpp>
#include <tuple>

namespace fgl
{
	template<typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		constexpr Vector2() : x(0), y(0) {
			//
		}

		constexpr Vector2(const T& x, const T& y) : x(x), y(y) {
			//
		}
		
		constexpr Vector2(const std::tuple<T,T>& tuple) : x(std::get<0>(tuple)), y(std::get<1>(tuple)) {
			//
		}

		template<typename U>
		constexpr explicit Vector2(const Vector2<U>& vect) : x(static_cast<T>(vect.x)), y(static_cast<T>(vect.y)) {
			//
		}

		template<typename _T=T, typename std::enable_if<std::is_signed<_T>::value, std::nullptr_t>::type = nullptr>
		constexpr Vector2<T> operator-() const {
			return Vector2<T>(-x, -y);
		}

		constexpr Vector2<T>& operator+=(const Vector2<T>& vect) {
			x += vect.x;
			y += vect.y;
			return *this;
		}

		constexpr Vector2<T>& operator-=(const Vector2<T>& vect) {
			x -= vect.x;
			y -= vect.y;
			return *this;
		}

		constexpr Vector2<T>& operator*=(const Vector2<T>& vect) {
			x *= vect.x;
			y *= vect.y;
			return *this;
		}

		constexpr Vector2<T>& operator/=(const Vector2<T>& vect) {
			x /= vect.x;
			y /= vect.y;
			return *this;
		}
		
		constexpr Vector2<T>& operator+=(const std::tuple<T,T>& tuple) {
			x += std::get<0>(tuple);
			y += std::get<1>(tuple);
			return *this;
		}
		
		constexpr Vector2<T>& operator-=(const std::tuple<T,T>& tuple) {
			x -= std::get<0>(tuple);
			y -= std::get<1>(tuple);
			return *this;
		}
		
		constexpr Vector2<T>& operator*=(const std::tuple<T,T>& tuple) {
			x *= std::get<0>(tuple);
			y *= std::get<1>(tuple);
			return *this;
		}
		
		constexpr Vector2<T>& operator/=(const std::tuple<T,T>& tuple) {
			x /= std::get<0>(tuple);
			y /= std::get<1>(tuple);
			return *this;
		}
		
		constexpr Vector2<T>& operator*=(const T& num) {
			x *= num;
			y *= num;
			return *this;
		}
		
		constexpr Vector2<T>& operator/=(const T& num) {
			x /= num;
			y /= num;
			return *this;
		}
		
		constexpr bool operator==(const Vector2<T>& vect) const {
			if (x == vect.x && y == vect.y) {
				return true;
			}
			return false;
		}

		constexpr bool operator!=(const Vector2<T>& vect) const {
			if (x == vect.x && y == vect.y) {
				return false;
			}
			return true;
		}

		constexpr bool equals(const Vector2<T>& vect) const {
			if (x == vect.x && y == vect.y) {
				return true;
			}
			return false;
		}

		constexpr T cross(const Vector2<T>& vect) const {
			return (x*vect.y) - (y*vect.x);
		}

		constexpr T dot(const Vector2<T>& vect) const {
			return (x*vect.x) + (y*vect.y);
		}
		
		String toString() const {
			return "Vector2(x:" + fgl::stringify<T>(x) + ", y:" + fgl::stringify<T>(y) + ")";
		}
	};
	
	
	
	template<typename T>
	constexpr Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right) {
		return Vector2<T>(left.x+right.x, left.y+right.y);
	}
	
	template<typename T>
	constexpr Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right) {
		return Vector2<T>(left.x-right.x, left.y-right.y);
	}
	
	template<typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& left, const Vector2<T>& right) {
		return Vector2<T>(left.x*right.x, left.y*right.y);
	}
	
	template<typename T>
	constexpr Vector2<T> operator/(const Vector2<T>& left, const Vector2<T>& right) {
		return Vector2<T>(left.x/right.x, left.y/right.y);
	}
	
	template<typename T>
	constexpr Vector2<T> operator+(const Vector2<T>& left, const std::tuple<T,T>& right) {
		return Vector2<T>(left.x+std::get<0>(right), left.y+std::get<1>(right));
	}
	
	template<typename T>
	constexpr Vector2<T> operator-(const Vector2<T>& left, const std::tuple<T,T>& right) {
		return Vector2<T>(left.x-std::get<0>(right), left.y-std::get<1>(right));
	}
	
	template<typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& left, const std::tuple<T,T>& right) {
		return Vector2<T>(left.x*std::get<0>(right), left.y*std::get<1>(right));
	}
	
	template<typename T>
	constexpr Vector2<T> operator/(const Vector2<T>& left, const std::tuple<T,T>& right) {
		return Vector2<T>(left.x/std::get<0>(right), left.y/std::get<1>(right));
	}
	
	template<typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& left, const T& right) {
		return Vector2<T>(left.x*right, left.y*right);
	}
	
	template<typename T>
	constexpr Vector2<T> operator/(const Vector2<T>& left, const T& right) {
		return Vector2<T>(left.x/right, left.y/right);
	}
	
	
	
	typedef Vector2<byte> Vector2b;
	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<unsigned int> Vector2u;
	typedef Vector2<long> Vector2l;
	typedef Vector2<long long> Vector2ll;
	typedef Vector2<unsigned long long> Vector2ull;
	typedef Vector2<long double> Vector2ld;
}
