
#pragma once

#include <functional>
#include <memory>
#ifdef _HAS_STD_OPTIONAL
	#include <optional>
#endif

namespace fgl
{
#define funcwrap(...) [&]()->auto { return __VA_ARGS__; }

	bool attempt(const std::function<void()>& block);
	
#ifdef _HAS_STD_OPTIONAL
	template<typename T>
	std::optional<T> attempt(std::function<std::optional<T>()> block)
	{
		try {
			return block();
		}
		catch(...) {
			return {};
		}
	}
	
	template<typename T>
	std::optional<T> attempt(std::function<T()> block)
	{
		try {
			return std::optional<T>(block());
		}
		catch(...) {
			return {};
		}
	}
#endif

#define define_shared(TYPENAME) \
	typedef std::shared_ptr<TYPENAME> $##TYPENAME; \
	template<typename...Args> \
	$##TYPENAME new_$##TYPENAME(Args&&... args) { \
		return std::make_shared<TYPENAME>(std::forward<Args>(args)...); \
	}
	
	template<typename T>
	inline std::shared_ptr<T> share(T* value) {
		return std::shared_ptr<T>(value);
	}
	
	#define $new(...) fgl::share(new __VA_ARGS__)
}
