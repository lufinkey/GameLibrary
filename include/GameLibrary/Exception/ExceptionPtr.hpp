
#pragma once

#include <exception>

namespace fgl
{
	class ExceptionPtr
	{
	public:
		template<typename E>
		ExceptionPtr(E e) : except_ptr(std::make_exception_ptr(e))
		{
			//
		}
		
		ExceptionPtr(std::exception_ptr eptr) : except_ptr(eptr)
		{
			//
		}
		
		ExceptionPtr() : except_ptr()
		{
			//
		}
		
		operator std::exception_ptr() const
		{
			return except_ptr;
		}
		
		std::exception_ptr ptr() const
		{
			return except_ptr;
		}
		
		void rethrow() const
		{
			std::rethrow_exception(except_ptr);
		}
		
	private:
		std::exception_ptr except_ptr;
	};
}
