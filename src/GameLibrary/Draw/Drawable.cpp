
#include <GameLibrary/Draw/Drawable.hpp>

namespace fgl
{
	Drawable::Drawable() {
		//
	}
	
	String Drawable::getDebugDescription() const {
		return "Drawable";
	}
	
	bool Drawable::getFlag(const String& flag) const {
		if(flag=="Drawable") {
			return true;
		}
		return false;
	}
	
	bool Drawable::shouldDraw(const DrawManager* drawManager) const {
		return true;
	}
}
