
#include <GameLibrary/Utilities/Number.h>

namespace GameLibrary
{
	Number::Number()
		: ptr(new Derived<long long>(0LL))
	{
		//
	}
	
	Number::Number(const Number& number)
		: ptr(number.ptr->clone())
	{
		//
	}
	
	Number::Number(Number&& number)
		: ptr(number.ptr)
	{
		number.ptr = nullptr;
	}
	
	Number::Number(Base* base)
		: ptr(base)
	{
		//
	}
	
	Number::~Number()
	{
		if(ptr!=nullptr)
		{
			delete ptr;
		}
	}
	
	Number& Number::operator=(const Number& number)
	{
		Base* old_ptr = ptr;
		ptr = number.ptr->clone();
		delete old_ptr;
		return *this;
	}
	
	Number& Number::operator=(Number&& number)
	{
		Base* old_ptr = ptr;
		ptr = number.ptr;
		number.ptr = nullptr;
		delete old_ptr;
		return *this;
	}
	
	void Number::selectOptimalType(const Number& right)
	{
		size_t r_size = right.ptr->getNumberOfBytes();
		size_t size = ptr->getNumberOfBytes();
		bool r_fp = right.ptr->isFloatingPoint();
		bool fp = ptr->isFloatingPoint();
		bool r_uns = right.ptr->isUnsigned();
		bool uns = ptr->isUnsigned();
		if(size < r_size)
		{
			if(r_fp || (r_uns && fp))
			{
				Base* old_ptr = ptr;
				ptr = new Derived<long double>(old_ptr->to<long double>());
				delete old_ptr;
			}
			else
			{
				Base* old_ptr = ptr;
				ptr = new Derived<long long>(old_ptr->to<long long>());
				delete old_ptr;
			}
		}
		else if(r_fp && !fp)
		{
			Base* old_ptr = ptr;
			ptr = new Derived<long double>(old_ptr->to<long double>());
			delete old_ptr;
		}
		else if(!r_uns && uns)
		{
			if(r_fp)
			{
				Base* old_ptr = ptr;
				ptr = new Derived<long double>(old_ptr->to<long double>());
				delete old_ptr;
			}
			else
			{
				Base* old_ptr = ptr;
				ptr = new Derived<long long>(old_ptr->to<long long>());
				delete old_ptr;
			}
		}
	}
	
	Number& Number::operator+=(const Number& number)
	{
		selectOptimalType(number);
		ptr->add(number.ptr);
		return *this;
	}
	
	Number& Number::operator-=(const Number& number)
	{
		selectOptimalType(number);
		ptr->subtract(number.ptr);
		return *this;
	}
	
	Number& Number::operator*=(const Number& number)
	{
		selectOptimalType(number);
		ptr->multiply(number.ptr);
		return *this;
	}
	
	Number& Number::operator/=(const Number& number)
	{
		selectOptimalType(number);
		ptr->divide(number.ptr);
		return *this;
	}
	
	Number& Number::operator%=(const Number& number)
	{
		selectOptimalType(number);
		ptr->mod(number.ptr);
		return *this;
	}
	
	Number& Number::operator++()
	{
		ptr->increment();
		return *this;
	}
	
	Number& Number::operator--()
	{
		ptr->decrement();
		return *this;
	}
	
	Number Number::operator-() const
	{
		Base* base = ptr->toSigned();
		base->neg();
		return Number(base);
	}
	
	int Number::compare(const Number& num) const
	{
		return ptr->compare(num.ptr);
	}
	
	bool Number::equals(const Number& num) const
	{
		return (ptr->compare(num.ptr) == 0);
	}
	
	bool Number::isBool() const
	{
		return ptr->isBool();
	}
	
	bool Number::isIntegral() const
	{
		return ptr->isIntegral();
	}
	
	bool Number::isFloatingPoint() const
	{
		return ptr->isFloatingPoint();
	}
	
	bool Number::isSigned() const
	{
		return ptr->isSigned();
	}
	
	bool Number::isUnsigned() const
	{
		return ptr->isUnsigned();
	}
	
	String Number::toString() const
	{
		return ptr->toString();
	}
	
	std::ostream& operator<<(std::ostream& stream, const Number& num)
	{
		return num.ptr->stream(stream);
	}
	
	Number operator+(const Number& left, const Number& right)
	{
		Number newNum = left;
		newNum += right;
		return newNum;
	}
	
	Number operator-(const Number& left, const Number& right)
	{
		Number newNum = left;
		newNum -= right;
		return newNum;
	}
	
	Number operator*(const Number& left, const Number& right)
	{
		Number newNum = left;
		newNum *= right;
		return newNum;
	}
	
	Number operator/(const Number& left, const Number& right)
	{
		Number newNum = left;
		newNum /= right;
		return newNum;
	}
	
	Number operator%(const Number& left, const Number& right)
	{
		Number newNum = left;
		newNum %= right;
		return newNum;
	}
	
	bool operator<(const Number& left, const Number& right)
	{
		return (left.compare(right) < 0);
	}
	
	bool operator<=(const Number& left, const Number& right)
	{
		return (left.compare(right) <= 0);
	}
	
	bool operator>(const Number& left, const Number& right)
	{
		return (left.compare(right) > 0);
	}
	
	bool operator>=(const Number& left, const Number& right)
	{
		return (left.compare(right) >= 0);
	}
	
	bool operator==(const Number& left, const Number& right)
	{
		return left.equals(right);
	}
	
	bool operator!=(const Number& left, const Number& right)
	{
		return !left.equals(right);
	}
}
