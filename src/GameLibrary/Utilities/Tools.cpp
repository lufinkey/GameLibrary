
#include <GameLibrary/Utilities/Tools.hpp>
#include <iostream>
#include <exception>

namespace fgl
{
	bool attempt(const std::function<void()>& block)
	{
		try
		{
			block();
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
}
