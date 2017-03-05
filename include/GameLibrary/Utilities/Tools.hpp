
#pragma once

#include <functional>
#include <memory>
#ifdef _HAS_STD_OPTIONAL
	#include <optional>
#endif

namespace fgl
{
	#define funcwrap(...) [&]( __VA_ARGS__ )->auto

	bool attempt(const std::function<void()>& block);
	
#ifdef _HAS_STD_OPTIONAL
	template<typename T>
	std::optional<T> attempt(std::function<std::optional<T>()> block)
	{
		try
		{
			return block();
		}
		catch(...)
		{
			return {};
		}
	}
	
	template<typename T>
	std::optional<T> attempt(std::function<T()> block)
	{
		try
		{
			return std::optional<T>(block());
		}
		catch(...)
		{
			return {};
		}
	}
#endif

	template<typename T>
	std::shared_ptr<T> share(T* value)
	{
		return std::shared_ptr<T>(value);
	}
}
