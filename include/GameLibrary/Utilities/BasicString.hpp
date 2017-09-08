
#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <locale>
#include <regex>
#include <type_traits>
#ifdef _STRING_STANDALONE
#include <vector>
#else
#include <GameLibrary/Types.hpp>
#endif

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

#ifndef _STRING_STANDALONE
namespace fgl
{
#endif
	template<typename CHAR_TYPE>
	class BasicString;
	
	class BasicStringUtils
	{
	public:
		template<typename T1, typename T2,
			bool SAME_SIZE = (sizeof(T1)==sizeof(T2))>
		struct same_size
		{
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct same_size<T1, T2, true>
		{
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T1, typename T2>
		struct is_same
		{
			static constexpr bool value = false;
		};
		template<typename T>
		struct is_same<T, T>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T,
			bool CONVERTABLE = (std::is_integral<T>::value && (sizeof(T)<=4))>
		struct can_convert_string_type
		{
			static constexpr bool value = false;
		};
		template<typename T>
		struct can_convert_string_type<T, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T,
			bool NOT_CONVERTABLE = (!std::is_integral<T>::value || (sizeof(T)>4))>
		struct cant_convert_string_type
		{
			static constexpr bool value = false;
		};
		template<typename T>
		struct cant_convert_string_type<T, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T1, typename T2,
			bool CONVERTABLE = (is_same<T1,T2>::value || (can_convert_string_type<T1>::value && can_convert_string_type<T2>::value))>
		struct can_convert_string_types
		{
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct can_convert_string_types<T1, T2, true>
		{
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T>
		struct is_char_type
		{
			static constexpr bool value = false;
		};
		
		template<typename T, bool CHAR_TYPE = !is_char_type<T>::value>
		struct is_not_char_type
		{
			static constexpr bool value = false;
		};
		
		template<typename T>
		struct is_not_char_type<T, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T1, typename T2,
			bool TEST = (!is_same<T1, T2>::value && same_size<T1,T2>::value && can_convert_string_types<T1, T2>::value)>
		struct same_size_convertable_strings
		{
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct same_size_convertable_strings<T1, T2, true>
		{
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T1, typename T2,
			bool TEST = (!is_same<T1, T2>::value && !same_size<T1, T2>::value && can_convert_string_types<T1, T2>::value)>
		struct diff_size_convertable_strings
		{
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct diff_size_convertable_strings<T1, T2, true>
		{
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename CHAR_TYPE,
			bool TEST = (same_size_convertable_strings<T,CHAR_TYPE>::value && is_char_type<CHAR_TYPE>::value)>
		struct same_size_convertable_with_char_type
		{
			static constexpr bool value = false;
		};
		template<typename T, typename CHAR_TYPE>
		struct same_size_convertable_with_char_type<T, CHAR_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef CHAR_TYPE char_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename CHAR_TYPE,
			bool TEST = (diff_size_convertable_strings<T, CHAR_TYPE>::value && is_char_type<CHAR_TYPE>::value)>
		struct diff_size_convertable_with_char_type
		{
			static constexpr bool value = false;
		};
		template<typename T, typename CHAR_TYPE>
		struct diff_size_convertable_with_char_type<T, CHAR_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef CHAR_TYPE char_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename CHAR_TYPE,
			bool TEST = (can_convert_string_types<T, CHAR_TYPE>::value && (is_char_type<CHAR_TYPE>::value || is_same<T, CHAR_TYPE>::value))>
		struct string_type_convertable_with_char_type
		{
			static constexpr bool value = false;
		};
		template<typename T, typename CHAR_TYPE>
		struct string_type_convertable_with_char_type<T, CHAR_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef CHAR_TYPE char_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename BOOL_TYPE,
			bool TEST = (is_same<BOOL_TYPE, bool>::value && !is_same<T, BOOL_TYPE>::value && std::is_integral<T>::value)>
		struct string_type_convertable_with_bool
		{
			static constexpr bool value = false;
		};
		template<typename T, typename BOOL_TYPE>
		struct string_type_convertable_with_bool<T, BOOL_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef BOOL_TYPE bool_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename NUM_TYPE,
			bool TEST = (std::is_arithmetic<NUM_TYPE>::value
						&& !is_same<NUM_TYPE, bool>::value && !is_same<T,NUM_TYPE>::value
						&& std::is_pod<T>::value && !is_char_type<NUM_TYPE>::value && !std::is_pointer<NUM_TYPE>::value)>
		struct string_type_convertable_with_number
		{
			static constexpr bool value = false;
		};
		template<typename T, typename NUM_TYPE>
		struct string_type_convertable_with_number<T, NUM_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef NUM_TYPE number_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename T, typename NUM_TYPE,
			bool TEST = ((std::is_arithmetic<NUM_TYPE>::value || std::is_enum<NUM_TYPE>::value)
				&& !is_same<NUM_TYPE, bool>::value && !is_same<T, NUM_TYPE>::value
				&& std::is_pod<T>::value && !is_char_type<NUM_TYPE>::value && !std::is_pointer<NUM_TYPE>::value)>
		struct string_type_convertable_with_number_or_enum
		{
			static constexpr bool value = false;
		};
		template<typename T, typename NUM_TYPE>
		struct string_type_convertable_with_number_or_enum<T, NUM_TYPE, true>
		{
			static constexpr bool value = true;
			typedef T type;
			typedef NUM_TYPE number_type;
			typedef std::nullptr_t null_type;
		};
		
		template<typename CHAR_TYPE>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE>
		static void string_plus(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const std::basic_string<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE>
		static void string_plus(const std::basic_string<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		#ifdef __OBJC__
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(CHAR_TYPE) && sizeof(CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, NSString* right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, NSString* right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(CHAR_TYPE) && sizeof(CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, NSString* right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(CHAR_TYPE) && sizeof(CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(NSString* left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(NSString* left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(CHAR_TYPE) && sizeof(CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static void string_plus(NSString* left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		#endif
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const OTHER_CHAR_TYPE* right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const OTHER_CHAR_TYPE* right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const CHAR_TYPE* left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const CHAR_TYPE* left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const OTHER_CHAR_TYPE& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const OTHER_CHAR_TYPE& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const SAME_CHAR_TYPE& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const CHAR_TYPE& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const CHAR_TYPE& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const CHAR_TYPE& left, const BasicString<SAME_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename BOOL_TYPE, typename BasicStringUtils::string_type_convertable_with_bool<CHAR_TYPE, BOOL_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const BOOL_TYPE& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename BOOL_TYPE, typename BasicStringUtils::string_type_convertable_with_bool<CHAR_TYPE, BOOL_TYPE>::null_type = nullptr>
		static void string_plus(const BOOL_TYPE& left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number_or_enum<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const NUM_TYPE& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number_or_enum<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
		static void string_plus(const NUM_TYPE& left, const BasicString<CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const std::basic_string<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const BasicString<CHAR_TYPE>& left, const std::basic_string<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const std::basic_string<CHAR_TYPE>& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
		template<typename CHAR_TYPE, typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		static void string_plus(const std::basic_string<CHAR_TYPE>& left, const BasicString<OTHER_CHAR_TYPE>& right, BasicString<CHAR_TYPE>* output);
	};
	
	template<> struct BasicStringUtils::is_char_type<char> { static constexpr bool value = true; typedef char type; typedef std::nullptr_t null_type; };
	template<> struct BasicStringUtils::is_char_type<wchar_t> { static constexpr bool value = true; typedef wchar_t type; typedef std::nullptr_t null_type; };
	template<> struct BasicStringUtils::is_char_type<char16_t> { static constexpr bool value = true;  typedef char16_t type; typedef std::nullptr_t null_type; };
	template<> struct BasicStringUtils::is_char_type<char32_t> { static constexpr bool value = true;  typedef char32_t type; typedef std::nullptr_t null_type; };
	#ifdef __OBJC__
		template<> struct BasicStringUtils::is_char_type<unichar> { static constexpr bool value = true; typedef unichar type; typedef std::nullptr_t null_type; };
	#endif
	
	
	template<typename CHAR_TYPE>
	class BasicString
	{
		template<typename OTHER_STRING_TYPE>
		friend class BasicString;
		
		friend class BasicStringUtils;
		
	public:
		typedef CHAR_TYPE char_type;
		typedef BasicString<CHAR_TYPE> string_type;
		static constexpr size_t NOT_FOUND = (size_t)-1;
		
		static size_t strlen(const CHAR_TYPE* str);
		static bool streq(const CHAR_TYPE* left, const CHAR_TYPE* right);
		
		BasicString();
		BasicString(const CHAR_TYPE* str, size_t length);
		BasicString(const CHAR_TYPE* str);
		BasicString(const BasicString<CHAR_TYPE>& str);
		BasicString(BasicString<CHAR_TYPE>&& str);
		BasicString(const std::basic_string<CHAR_TYPE>& str);
		
		#ifdef __OBJC__
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE,CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(_CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		#endif
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString(const OTHER_CHAR_TYPE& c);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString(const OTHER_CHAR_TYPE& c);
		template<typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		BasicString(const SAME_CHAR_TYPE& c);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString(const OTHER_CHAR_TYPE* str, size_t length);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString(const OTHER_CHAR_TYPE* str, size_t length);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const OTHER_CHAR_TYPE* str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const OTHER_CHAR_TYPE* str);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const std::basic_string<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		explicit BasicString(const std::basic_string<OTHER_CHAR_TYPE>& str);
		
		~BasicString();
		
		
		
		operator const CHAR_TYPE*() const;
		operator std::basic_string<CHAR_TYPE>() const;
		
		#ifdef __OBJC__
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(_CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		explicit operator NSString*() const;
		#endif
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		std::basic_string<OTHER_CHAR_TYPE> toStandardString() const;
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		std::basic_string<OTHER_CHAR_TYPE> toStandardString() const;
		template<typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		std::basic_string<SAME_CHAR_TYPE> toStandardString() const;
		
		template<typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
		BasicString<SOME_CHAR_TYPE> toBasicString() const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<CHAR_TYPE>::null_type = nullptr>
		BasicString<char> toString() const;
		
		
		
		void assign(const CHAR_TYPE* str, size_t length);
		void assign(const CHAR_TYPE* str);
		
		BasicString<CHAR_TYPE>& operator=(const CHAR_TYPE* str);
		BasicString<CHAR_TYPE>& operator=(const BasicString<CHAR_TYPE>& str);
		BasicString<CHAR_TYPE>& operator=(BasicString<CHAR_TYPE>&& str);
		BasicString<CHAR_TYPE>& operator=(const std::basic_string<CHAR_TYPE>& str);
		
		#ifdef __OBJC__
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator=(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(_CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator=(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator=(NSString* nsString);
		#endif
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const OTHER_CHAR_TYPE& c);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const OTHER_CHAR_TYPE& c);
		template<typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const SAME_CHAR_TYPE& c);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const OTHER_CHAR_TYPE* str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const OTHER_CHAR_TYPE* str);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const std::basic_string<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator=(const std::basic_string<OTHER_CHAR_TYPE>& str);
		
		
		
		void append(const CHAR_TYPE* str, size_t length);
		void append(const CHAR_TYPE* str);
		void append(const CHAR_TYPE& c);
		
		BasicString<CHAR_TYPE>& operator+=(const CHAR_TYPE* str);
		BasicString<CHAR_TYPE>& operator+=(const BasicString<CHAR_TYPE>& str);
		BasicString<CHAR_TYPE>& operator+=(const std::basic_string<CHAR_TYPE>& str);
		
		#ifdef __OBJC__
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)==sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(_CHAR_TYPE)==sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(NSString* nsString);
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::value
				&& sizeof(unichar)!=sizeof(_CHAR_TYPE) && sizeof(_CHAR_TYPE)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(NSString* nsString);
		#endif
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const OTHER_CHAR_TYPE& c);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_with_char_type<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const OTHER_CHAR_TYPE& c);
		template<typename SAME_CHAR_TYPE, typename BasicStringUtils::is_same<CHAR_TYPE, SAME_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const SAME_CHAR_TYPE& c);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const OTHER_CHAR_TYPE* str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const OTHER_CHAR_TYPE* str);
		
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const BasicString<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::same_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const std::basic_string<OTHER_CHAR_TYPE>& str);
		template<typename OTHER_CHAR_TYPE, typename BasicStringUtils::diff_size_convertable_strings<CHAR_TYPE, OTHER_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const std::basic_string<OTHER_CHAR_TYPE>& str);
		
		template<typename BOOL_TYPE, typename BasicStringUtils::string_type_convertable_with_bool<CHAR_TYPE, BOOL_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const BOOL_TYPE& b);
		template<typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number_or_enum<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE>& operator+=(const NUM_TYPE& num);
		
		
		
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE,CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		int compare(const CHAR_TYPE* cmp, size_t length) const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE,CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		int compare(const CHAR_TYPE* cmp, size_t length, const std::locale& locale) const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::cant_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		int compare(const CHAR_TYPE* cmp, size_t length) const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::cant_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		int compare(const CHAR_TYPE* cmp, size_t length, const std::locale& locale) const;
		
		int compare(const CHAR_TYPE* cmp) const;
		int compare(const CHAR_TYPE* cmp, const std::locale& locale) const;
		int compare(const BasicString<CHAR_TYPE>& cmp) const;
		int compare(const BasicString<CHAR_TYPE>& cmp, const std::locale& locale) const;
		
		bool equals(const CHAR_TYPE* str, size_t length) const;
		bool equals(const CHAR_TYPE* str) const;
		bool equals(const BasicString<CHAR_TYPE>& str) const;
		
		
		
		size_t length() const;
		void clear();
		void resize(size_t size);
		CHAR_TYPE& charAt(size_t index);
		CHAR_TYPE& operator[](size_t index);
		const CHAR_TYPE& charAt(size_t index) const;
		const CHAR_TYPE& operator[](size_t index) const;
		const CHAR_TYPE* getData() const;
		
		size_t indexOf(const CHAR_TYPE& find, size_t startIndex=0) const;
		size_t indexOf(const BasicString<CHAR_TYPE>& find, size_t startIndex=0) const;
		size_t indexOf(const CHAR_TYPE* find, size_t startIndex=0) const;
		size_t lastIndexOf(const CHAR_TYPE& find, size_t startIndex) const;
		size_t lastIndexOf(const CHAR_TYPE& find) const;
		size_t lastIndexOf(const BasicString<CHAR_TYPE>& find, size_t startIndex) const;
		size_t lastIndexOf(const BasicString<CHAR_TYPE>& find) const;
		size_t lastIndexOf(const CHAR_TYPE* find, size_t startIndex) const;
		size_t lastIndexOf(const CHAR_TYPE* find) const;

		bool startsWith(const CHAR_TYPE* str) const;
		bool startsWith(const CHAR_TYPE* str, size_t length) const;
		bool startsWith(const BasicString<CHAR_TYPE>& str) const;
		bool endsWith(const CHAR_TYPE* str) const;
		bool endsWith(const CHAR_TYPE* str, size_t length) const;
		bool endsWith(const BasicString<CHAR_TYPE>& str) const;
		
		BasicString<CHAR_TYPE> replace(const CHAR_TYPE& find, const CHAR_TYPE& replace) const;
		BasicString<CHAR_TYPE> replace(const BasicString<CHAR_TYPE>& find, const BasicString<CHAR_TYPE>& replace) const;
		BasicString<CHAR_TYPE> replace(const std::basic_regex<CHAR_TYPE>& find, const BasicString<CHAR_TYPE>& replace) const;
		BasicString<CHAR_TYPE> replace(size_t startIndex, size_t endIndex, const CHAR_TYPE* replace) const;
		BasicString<CHAR_TYPE> replace(size_t startIndex, size_t endIndex, const BasicString<CHAR_TYPE>& replace) const;
		
		BasicString<CHAR_TYPE> substring(size_t startIndex, size_t endIndex) const;
		BasicString<CHAR_TYPE> substring(size_t startIndex) const;
		
		#ifndef _STRING_STANDALONE
		ArrayList<BasicString<CHAR_TYPE>> split(const CHAR_TYPE& delim) const;
		ArrayList<BasicString<CHAR_TYPE>> split(const CHAR_TYPE* delim) const;
		ArrayList<BasicString<CHAR_TYPE>> split(const BasicString<CHAR_TYPE>& delim) const;
		#else
		std::vector<BasicString<CHAR_TYPE>> split(const CHAR_TYPE& delim) const;
		std::vector<BasicString<CHAR_TYPE>> split(const CHAR_TYPE* delim) const;
		std::vector<BasicString<CHAR_TYPE>> split(const BasicString<CHAR_TYPE>& delim) const;
		#endif
		
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE> trim(const std::locale& locale = std::locale()) const;
		
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE> toLowerCase(const std::locale& locale = std::locale()) const;
		template<typename _CHAR_TYPE=CHAR_TYPE,
			typename BasicStringUtils::is_same<_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_CHAR_TYPE>::null_type = nullptr>
		BasicString<CHAR_TYPE> toUpperCase(const std::locale& locale = std::locale()) const;
		
		template<typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
		NUM_TYPE toArithmeticValue(const std::locale& locale = std::locale()) const;
		
	private:
		CHAR_TYPE* characters;
		size_t size;
		
		
		template<typename T, size_t T_SIZE=sizeof(T)>
		struct utf_eqv {};
		template<typename T>
		struct utf_eqv<T, 1> { typedef char type; };
		#ifdef _WIN32
			template<typename T>
			struct utf_eqv<T, 2> { typedef int16_t type; };
			template<typename T>
			struct utf_eqv<T, 4> { typedef int32_t type; };
		#else
			template<typename T>
			struct utf_eqv<T, 2> { typedef char16_t type; };
			template<typename T>
			struct utf_eqv<T, 4> { typedef char32_t type; };
		#endif
		
		typedef typename utf_eqv<CHAR_TYPE>::type utf_eqv_t;
		
		template<typename NUM_TYPE>
		static NUM_TYPE ten_to(long the);
		
		template<typename _IN, typename std::enable_if<(sizeof(CHAR_TYPE)==sizeof(_IN)), bool>::type = true>
		static std::basic_string<CHAR_TYPE>& string_cast(std::basic_string<_IN>&& str);
		
		static void convert_fromBool(const bool& b, CHAR_TYPE* output, size_t* output_length);
		
		template<typename NUM_TYPE>
		static void convert_fromNumber(const NUM_TYPE& num, std::basic_string<CHAR_TYPE>* output);
		
		//same size chars
		template<typename INPUT_CHAR_TYPE,
			typename std::enable_if<((sizeof(CHAR_TYPE)==sizeof(INPUT_CHAR_TYPE)) && BasicStringUtils::can_convert_string_types<CHAR_TYPE,INPUT_CHAR_TYPE>::value), std::nullptr_t>::type = nullptr>
		static void convert_fromString(const INPUT_CHAR_TYPE* str, size_t length, std::basic_string<CHAR_TYPE>* output);
		//multibyte to byte
		template<typename INPUT_CHAR_TYPE,
			typename std::enable_if<((sizeof(CHAR_TYPE)==1 && sizeof(INPUT_CHAR_TYPE)>1) && BasicStringUtils::can_convert_string_types<CHAR_TYPE, INPUT_CHAR_TYPE>::value), std::nullptr_t>::type = nullptr>
		static void convert_fromString(const INPUT_CHAR_TYPE* str, size_t length, std::basic_string<CHAR_TYPE>* output);
		//multibyte to multibyte
		template<typename INPUT_CHAR_TYPE,
			typename std::enable_if<((sizeof(CHAR_TYPE)>1 && sizeof(INPUT_CHAR_TYPE)>1) && (sizeof(CHAR_TYPE)!=sizeof(INPUT_CHAR_TYPE))
				&& BasicStringUtils::can_convert_string_types<CHAR_TYPE, INPUT_CHAR_TYPE>::value), std::nullptr_t>::type = nullptr>
		static void convert_fromString(const INPUT_CHAR_TYPE* str, size_t length, std::basic_string<CHAR_TYPE>* output);
		//byte to multibyte
		template<typename INPUT_CHAR_TYPE,
			typename std::enable_if<((sizeof(CHAR_TYPE)>1 && sizeof(INPUT_CHAR_TYPE)==1) && BasicStringUtils::can_convert_string_types<CHAR_TYPE, INPUT_CHAR_TYPE>::value), std::nullptr_t>::type = nullptr>
		static void convert_fromString(const INPUT_CHAR_TYPE* str, size_t length, std::basic_string<CHAR_TYPE>* output);
		
		enum SymbolType
		{
			STRINGCMP_SYMTYPE_SYMBOL,
			STRINGCMP_SYMTYPE_NUMBER,
			STRINGCMP_SYMTYPE_UPPERCASE_LETTER,
			STRINGCMP_SYMTYPE_LOWERCASE_LETTER
		};
		
		static SymbolType getSymbolType(const CHAR_TYPE& c, const std::locale& locale = std::locale());
	};
	
	template<typename BASICSTRING_TYPE, typename CHAR_TYPE = typename BASICSTRING_TYPE::char_type,
		bool CHECK = std::is_same<BASICSTRING_TYPE, BasicString<CHAR_TYPE> >::value>
	struct is_BasicString
	{
		static constexpr bool value = false;
	};
	
	template<typename BASICSTRING_TYPE, typename CHAR_TYPE>
	struct is_BasicString<BASICSTRING_TYPE, CHAR_TYPE, true>
	{
		static constexpr bool value = true;
		typedef CHAR_TYPE char_type;
		typedef BasicString<CHAR_TYPE> string_type;
		typedef std::nullptr_t null_type;
	};
	
	
	
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const BasicString<SOME_CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const SOME_CHAR_TYPE* right);
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const CHAR_TYPE* left, const BasicString<SOME_CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const std::basic_string<SOME_CHAR_TYPE>& right);
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const std::basic_string<CHAR_TYPE>& left, const BasicString<SOME_CHAR_TYPE>& right);
	
	#ifdef __OBJC__
	template<typename CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, unichar>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, NSString* right);
	template<typename CHAR_TYPE, typename BasicStringUtils::can_convert_string_types<CHAR_TYPE, unichar>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(NSString* left, const BasicString<CHAR_TYPE>& right);
	#endif
	
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::string_type_convertable_with_char_type<CHAR_TYPE, SOME_CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const SOME_CHAR_TYPE& right);
	template<typename CHAR_TYPE, typename SOME_CHAR_TYPE, typename BasicStringUtils::string_type_convertable_with_char_type<SOME_CHAR_TYPE, CHAR_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const CHAR_TYPE& left, const BasicString<SOME_CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE, typename BOOL_TYPE, typename BasicStringUtils::string_type_convertable_with_bool<CHAR_TYPE, BOOL_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const BOOL_TYPE& right);
	template<typename CHAR_TYPE, typename BOOL_TYPE, typename BasicStringUtils::string_type_convertable_with_bool<CHAR_TYPE, BOOL_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BOOL_TYPE& left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE, typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number_or_enum<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const BasicString<CHAR_TYPE>& left, const NUM_TYPE& right);
	template<typename CHAR_TYPE, typename NUM_TYPE, typename BasicStringUtils::string_type_convertable_with_number_or_enum<CHAR_TYPE, NUM_TYPE>::null_type = nullptr>
	BasicString<CHAR_TYPE> operator+(const NUM_TYPE& left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator==(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator==(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator==(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator==(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE& right);
	template<typename CHAR_TYPE>
	bool operator==(const CHAR_TYPE& left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator!=(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator!=(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator!=(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator!=(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE& right);
	template<typename CHAR_TYPE>
	bool operator!=(const CHAR_TYPE& left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator<(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator<(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator<(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator<=(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator<=(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator<=(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator>(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator>(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator>(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	
	template<typename CHAR_TYPE>
	bool operator>=(const BasicString<CHAR_TYPE>& left, const BasicString<CHAR_TYPE>& right);
	template<typename CHAR_TYPE>
	bool operator>=(const BasicString<CHAR_TYPE>& left, const CHAR_TYPE* right);
	template<typename CHAR_TYPE>
	bool operator>=(const CHAR_TYPE* left, const BasicString<CHAR_TYPE>& right);
	
	typedef BasicString<std::string::value_type> String;
	typedef BasicString<std::wstring::value_type> WideString;

#ifndef _STRING_STANDALONE
}
#endif
