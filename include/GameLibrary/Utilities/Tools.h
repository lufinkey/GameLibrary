
#pragma once

#include <functional>

namespace fgl
{
	bool attempt(std::function<void()> block);

#define func(...) [=](__VA_ARGS__)
#define imfunc(...) [&](__VA_ARGS__)
}
