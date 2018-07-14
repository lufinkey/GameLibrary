
#include <GameLibrary/Utilities/Aspectable.hpp>

namespace fgl
{
	Aspectable::~Aspectable()
	{
		for(auto& aspectPair : aspects) {
			for(auto aspect : aspectPair.second) {
				aspect->release();
			}
		}
	}
}
