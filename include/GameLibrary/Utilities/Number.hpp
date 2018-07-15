
#pragma once

#include <type_traits>
#include <GameLibrary/Types.hpp>
#include <GameLibrary/Exception/Utilities/BadNumberCastException.hpp>
#include "TypeRegistry.hpp"
#include "String.hpp"

#ifdef check
#undef check
#endif

namespace fgl
{
	class Number
	{
		friend std::ostream& operator<<(std::ostream& stream, const Number& num);
	public:
		Number();
		Number(const Number&);
		Number(Number&&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number(const T&);
		~Number();
		
		Number& operator=(const Number&);
		Number& operator=(Number&&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator=(const T&);
		Number& operator+=(const Number&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator+=(const T&);
		Number& operator-=(const Number&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator-=(const T&);
		Number& operator*=(const Number&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator*=(const T&);
		Number& operator/=(const Number&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator/=(const T&);
		Number& operator%=(const Number&);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		Number& operator%=(const T&);
		
		Number& operator++();
		Number& operator--();
		
		Number operator-() const;
		
		int compare(const Number&) const;
		bool equals(const Number&) const;
		
		bool isBool() const;
		bool isIntegral() const;
		bool isFloatingPoint() const;
		bool isSigned() const;
		bool isUnsigned() const;
		
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		operator T() const;
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T,bool>::value, std::nullptr_t>::type = nullptr>
		T toArithmeticValue() const;
		template<typename T, typename std::enable_if<std::is_same<T,bool>::value, std::nullptr_t>::type = nullptr>
		T toArithmeticValue() const;
		
		String toString() const;
		
	private:
		void selectOptimalType(const Number& right);
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		void selectOptimalType(const T& right);
		
		class Base
		{
		public:
			virtual ~Base() {}
			virtual bool check() const = 0;
			virtual void add(Base* base) = 0;
			virtual void subtract(Base* base) = 0;
			virtual void multiply(Base* base) = 0;
			virtual void divide(Base* base) = 0;
			virtual void mod(Base* base) = 0;
			virtual void increment() = 0;
			virtual void decrement() = 0;
			virtual void neg() = 0;
			virtual Base* toSigned() const = 0;
			virtual Base* clone() const = 0;
			
			virtual bool isNegative() const = 0;
			virtual int compare(Base* base) const = 0;
			
			virtual bool isBool() const = 0;
			virtual bool isIntegral() const = 0;
			virtual bool isFloatingPoint() const = 0;
			virtual bool isSigned() const = 0;
			virtual bool isUnsigned() const = 0;
			
			virtual void* getPtr() const = 0;
			virtual size_t getNumberOfBytes() const = 0;
			
			virtual String toString() const = 0;
			
			virtual std::ostream& stream(std::ostream& stream) const = 0;
			
			template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
			T to() const;
		};
		
		template<typename T>
		class Derived : public Base
		{
		public:
			Derived(const T& value);
			
			virtual bool check() const override;
			virtual void add(Base* base) override;
			virtual void subtract(Base* base) override;
			virtual void multiply(Base* base) override;
			virtual void divide(Base* base) override;
			virtual void mod(Base* base) override;
			virtual void increment() override;
			virtual void decrement() override;
			virtual void neg() override;
			virtual Base* toSigned() const override;
			virtual Base* clone() const override;
			
			virtual bool isNegative() const override;
			virtual int compare(Base* base) const override;

			virtual bool isBool() const override;
			virtual bool isIntegral() const override;
			virtual bool isFloatingPoint() const override;
			virtual bool isSigned() const override;
			virtual bool isUnsigned() const override;

			virtual void* getPtr() const override;
			virtual size_t getNumberOfBytes() const override;
			
			virtual String toString() const override;
			virtual std::ostream& stream(std::ostream& stream) const override;
			
		private:
			T value;
			
			template<typename U=T, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
			bool impl_check() const;
			template<typename U=T, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
			bool impl_check() const;
			
			template<typename U=T, typename std::enable_if<(std::is_arithmetic<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_add(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_add(Base* base);
			
			template<typename U=T, typename std::enable_if<(std::is_arithmetic<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_subtract(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_subtract(Base* base);
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			void impl_multiply(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_multiply(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_multiply(Base* base);
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			void impl_divide(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_divide(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_divide(Base* base);
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			void impl_mod(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_mod(Base* base);
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_mod(Base* base);
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			void impl_increment();
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_increment();
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_increment();
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			void impl_decrement();
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_decrement();
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			void impl_decrement();
			
			template<typename U=T, typename std::enable_if<(std::is_signed<U>::value), std::nullptr_t>::type = nullptr>
			void impl_neg();
			template<typename U=T, typename std::enable_if<(std::is_unsigned<U>::value), std::nullptr_t>::type = nullptr>
			void impl_neg();
			
			template<typename U=T, typename std::enable_if<(std::is_signed<U>::value), std::nullptr_t>::type = nullptr>
			Base* impl_toSigned() const;
			template<typename U=T, typename std::enable_if<(std::is_unsigned<U>::value), std::nullptr_t>::type = nullptr>
			Base* impl_toSigned() const;
			
			template<typename U=T, typename std::enable_if<std::is_same<U,bool>::value, std::nullptr_t>::type = nullptr>
			bool impl_isNegative() const;
			template<typename U=T, typename std::enable_if<!std::is_same<U, bool>::value, std::nullptr_t>::type = nullptr>
			bool impl_isNegative() const;
			
			template<typename U=T, typename std::enable_if<(std::is_floating_point<U>::value), std::nullptr_t>::type = nullptr>
			int impl_compare(Base* base) const;
			template<typename U=T, typename std::enable_if<(std::is_integral<U>::value && !std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			int impl_compare(Base* base) const;
			template<typename U=T, typename std::enable_if<(std::is_same<U, bool>::value), std::nullptr_t>::type = nullptr>
			int impl_compare(Base* base) const;
		};
		
		Base* ptr;
		
		Number(Base*);
	};
	
	Number operator+(const Number& left, const Number& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator+(const Number& left, const T& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator+(const T& left, const Number& right);
	
	Number operator-(const Number& left, const Number& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator-(const Number& left, const T& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator-(const T& left, const Number& right);
	
	Number operator*(const Number& left, const Number& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator*(const Number& left, const T& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator*(const T& left, const Number& right);
	
	Number operator/(const Number& left, const Number& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator/(const Number& left, const T& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator/(const T& left, const Number& right);
	
	Number operator%(const Number& left, const Number& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator%(const Number& left, const T& right);
	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
	Number operator%(const T& left, const Number& right);
	
	bool operator<(const Number& left, const Number& right);
	bool operator<=(const Number& left, const Number& right);
	bool operator>(const Number& left, const Number& right);
	bool operator>=(const Number& left, const Number& right);
	bool operator==(const Number& left, const Number& right);
	bool operator!=(const Number& left, const Number& right);
}

#include "Number.impl"

REGISTER_TYPE(fgl::Number)
