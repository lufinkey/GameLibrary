
#pragma once

#include <functional>
#ifdef _HAS_STD_OPTIONAL
	#include <optional>
#endif

namespace fgl
{
	#define funcwrap [&]()->auto

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
}
