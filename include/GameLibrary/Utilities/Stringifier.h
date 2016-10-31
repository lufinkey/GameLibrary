
#pragma once

#include <type_traits>
#include <iomanip>
#include <sstream>
#include "BasicString.h"

namespace fgl
{
	namespace StringifyUtils
	{
		template<typename T, typename T_FUNC, typename std::enable_if<std::is_class<T>::value, std::nullptr_t>::type = nullptr>
		struct _has_toString
		{
		private:
			typedef char yes[1];
			typedef char no[2];
			template <typename U, U> struct type_check;
			template <typename Q> static yes &chk(type_check<T_FUNC, &Q::toString> *);
			template <typename  > static no  &chk(...);

		public:
			static constexpr bool value = sizeof(chk<T>(0)) == sizeof(yes);
		};

		template<typename T, typename T_FUNC, typename std::enable_if<std::is_class<T>::value, std::nullptr_t>::type = nullptr>
		struct _has_to_string
		{
		private:
			typedef char yes[1];
			typedef char no[2];
			template <typename U, U> struct type_check;
			template <typename Q> static yes &chk(type_check<T_FUNC, &Q::to_string> *);
			template <typename  > static no  &chk(...);

		public:
			static constexpr bool value = sizeof(chk<T>(0)) == sizeof(yes);
		};

		template<typename T,
			bool CHECK = (_has_toString<T, String(T::*)()const>::value
				|| _has_toString<T, std::string(T::*)()const>::value
				|| _has_toString<T, char*(T::*)()const>::value
				|| _has_toString<T, const char*(T::*)()const>::value)>
		struct has_toString
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct has_toString<T, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};

		template<typename T,
			bool CHECK = !has_toString<T>::value
				&& (_has_to_string<T, String(T::*)()const>::value
				|| _has_to_string<T, std::string(T::*)()const>::value
				|| _has_to_string<T, char*(T::*)()const>::value
				|| _has_to_string<T, const char*(T::*)()const>::value)>
		struct has_to_string
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct has_to_string<T, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, bool HAS_NO_CONVERSION_FUNC = std::is_class<T>::value && !(has_toString<T>::value || has_to_string<T>::value)>
		struct is_class_without_toString_or_to_string
		{
			static constexpr bool value = false;
		};
		
		template<typename T>
		struct is_class_without_toString_or_to_string<T,true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename StringifyUtils::has_toString<T>::null_type = nullptr>
		String stringify(const T* val)
		{
			return val->toString();
		}
		
		template<typename T, typename StringifyUtils::has_to_string<T>::null_type = nullptr>
		String stringify(const T* val)
		{
			return val->to_string();
		}
		
		template<typename T,
			typename std::enable_if<(
				std::is_same<T, std::string>::value
				|| std::is_same<T, std::wstring>::value
				|| std::is_same<T, std::basic_string<char16_t> >::value
				|| std::is_same<T, std::basic_string<char32_t> >::value), std::nullptr_t>::type = nullptr>
		String stringify(const T* val)
		{
			return *val;
		}
		
		template<typename T,
			typename StringifyUtils::is_class_without_toString_or_to_string<T>::null_type = nullptr,
			typename std::enable_if<(
				!std::is_same<T, std::string>::value
				&& !std::is_same<T, std::wstring>::value
				&& !std::is_same<T, std::basic_string<char16_t> >::value
				&& !std::is_same<T, std::basic_string<char32_t> >::value), std::nullptr_t>::type = nullptr>
		String stringify(const T* val)
		{
			if(val==nullptr)
			{
				return String("null");
			}
			std::stringstream ss;
			ss << typeid(T).name() << " @ 0x" << std::setfill('0') << std::setw(sizeof(val)*2) << std::hex << ((uintptr_t)val);
			return ss.str();
		}
		
		template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
		String stringify(const T* val)
		{
			String str;
			str += *val;
			return str;
		}
		
		template<typename T,
			typename std::enable_if<std::is_pointer<T>::value, std::nullptr_t>::type = nullptr,
			typename BasicStringUtils::is_char_type<typename std::remove_pointer<typename std::remove_const<T>::type>::type>::null_type = nullptr>
		String stringify(const T* val)
		{
			return *val;
		}
		
		template<typename T,
			typename std::enable_if<std::is_pointer<T>::value, std::nullptr_t>::type = nullptr,
			typename BasicStringUtils::is_not_char_type<typename std::remove_pointer<typename std::remove_const<T>::type>::type>::null_type = nullptr>
		String stringify(const T* val)
		{
			if((*val)==nullptr)
			{
				return String("null");
			}
			std::stringstream ss;
			ss << typeid(T).name() << " @ 0x" << std::setfill('0') << std::setw(sizeof(*val)*2) << std::hex << ((uintptr_t)*val);
			return ss.str();
		}
	}
	
	template<typename T>
	String stringify(const T& val)
	{
		return StringifyUtils::stringify<typename std::decay<T>::type>(&val);
	}
}
