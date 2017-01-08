
#pragma once

#include <functional>
#include <optional>

namespace fgl
{
#define lambda(...) [=](__VA_ARGS__)
#define immlambda(...) [&](__VA_ARGS__)
#define autolambda(...) [&](__VA_ARGS__) -> auto

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
			return block();
		}
		catch(...)
		{
			return {};
		}
	}
}
