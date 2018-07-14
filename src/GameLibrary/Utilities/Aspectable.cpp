
#include <GameLibrary/Utilities/Aspectable.hpp>

namespace fgl
{
	bool Aspect::getFlag(const String& flag)
	{
		return false;
	}
	
	Aspectable::~Aspectable()
	{
		for(auto& aspectPair : aspects) {
			for(auto aspect : aspectPair.second) {
				aspect->release();
			}
		}
	}
}
