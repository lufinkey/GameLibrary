
#pragma once

#include <exception>

namespace fgl
{
	class ExceptionPtr
	{
	public:
		ExceptionPtr(const std::exception_ptr& eptr) : except_ptr(eptr) {
			//
		}
		
		ExceptionPtr(std::exception_ptr&& eptr) : except_ptr(eptr) {
			//
		}
		
		ExceptionPtr(const ExceptionPtr& eptr) : except_ptr(eptr.except_ptr) {
			//
		}
		
		ExceptionPtr(ExceptionPtr&& eptr) : except_ptr(eptr.except_ptr) {
			//
		}
		
		template<
			typename E,
			typename E_TYPE = typename std::remove_reference<typename std::remove_cv<E>::type>::type,
			typename std::enable_if<
				!std::is_same<E_TYPE,std::exception_ptr>::value
				&& !std::is_same<E_TYPE,ExceptionPtr>::value, std::nullptr_t>::type = nullptr>
		ExceptionPtr(const E& e) : except_ptr(std::make_exception_ptr(e)) {
			//
		}
		
		template<
			typename E,
			typename E_TYPE = typename std::remove_reference<typename std::remove_cv<E>::type>::type,
			typename std::enable_if<
				!std::is_same<E_TYPE,std::exception_ptr>::value
				&& !std::is_same<E_TYPE,ExceptionPtr>::value, std::nullptr_t>::type = nullptr>
		ExceptionPtr(E&& e) : except_ptr(std::make_exception_ptr(e)) {
			//
		}
		
		ExceptionPtr() : except_ptr() {
			//
		}
		
		operator std::exception_ptr() const {
			return except_ptr;
		}
		
		std::exception_ptr ptr() const {
			return except_ptr;
		}
		
		void rethrow() const {
			std::rethrow_exception(except_ptr);
		}
		
	private:
		std::exception_ptr except_ptr;
	};
}
