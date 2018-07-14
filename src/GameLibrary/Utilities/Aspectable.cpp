
#include <GameLibrary/Utilities/Aspectable.hpp>

namespace fgl
{
	bool Aspect::getFlag(const String& flag) const
	{
		return false;
	}
	
	Aspectable::~Aspectable()
	{
		for(auto& aspectPair : aspects) {
			for(auto aspect : aspectPair.second) {
				delete aspect;
			}
		}
	}
	
	void Aspectable::onAddAspect(TypeRegistryId aspectType, Aspect* aspect)
	{
		// open for implementation
	}
}
