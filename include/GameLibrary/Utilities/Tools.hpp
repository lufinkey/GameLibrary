
#pragma once

#include <functional>
#include <memory>
#ifdef _HAS_STD_OPTIONAL
	#include <optional>
#endif
#include <cstdio>

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

	#define define_shared(TYPENAME) typedef std::shared_ptr<TYPENAME> $##TYPENAME;
	
	template<typename T, typename...Args> \
	inline std::shared_ptr<T> $new(Args&&... args) { \
		return std::make_shared<T>(std::forward<Args>(args)...); \
	}
	
	#define ASSERT(condition, message) { \
		if(!(condition)) { \
			fprintf(stderr, "Assertion `" #condition "` failed in %s line %i: %s\n", __FILE__, __LINE__, message); \
			abort(); \
		} \
	}
}
