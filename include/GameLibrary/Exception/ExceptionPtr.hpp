
#pragma once

#include <exception>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class ExceptionPtr
	{
	public:
		template<typename E, typename std::enable_if<!std::is_same<std::remove_reference<E>, std::exception_ptr>::value, std::nullptr_t> = nullptr>
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

REGISTER_TYPE(fgl::ExceptionPtr)
