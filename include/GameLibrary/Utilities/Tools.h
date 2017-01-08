
#pragma once

#include <functional>
#include <optional>

namespace fgl
{
	#define funcwrap [&]()->auto

	bool attempt(std::function<void()> block);
	
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
}
