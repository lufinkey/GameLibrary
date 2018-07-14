
#include <GameLibrary/Utilities/Retainable.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>

namespace fgl
{
	Retainable::Retainable() : retainCount(0)
	{
		//
	}
	
	size_t Retainable::retain()
	{
		retainCount++;
		return retainCount;
	}
	
	size_t Retainable::release()
	{
		if(retainCount <= 0) {
			throw IllegalStateException("Cannot call release without a prior call to retain");
		}
		retainCount--;
		auto count = retainCount;
		if(count == 0)
		{
			delete this;
		}
		return count;
	}
	
	size_t Retainable::getRetainCount() const
	{
		return retainCount;
	}
}
