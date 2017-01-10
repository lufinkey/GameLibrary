
#include <GameLibrary/Utilities/Tools.hpp>

namespace fgl
{
	bool attempt(std::function<void()> block)
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
