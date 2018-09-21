
#include <GameLibrary/World/Aspects/Drawing/Drawable2DAspect.hpp>
#include <GameLibrary/World/World.hpp>

namespace fgl
{
	String Drawable2DAspect::getDebugDescription() const {
		auto object = getObject();
		if(object != nullptr) {
			return "Drawable2DAspect of " + object->getDebugDescription();
		}
		return Drawable::getDebugDescription();
	}
	
	void Drawable2DAspect::onAddObjectToWorld(World* world) {
		world->addDrawable(this);
	}
	
	void Drawable2DAspect::onRemoveObjectFromWorld(World* world) {
		world->removeDrawable(this);
	}
}
