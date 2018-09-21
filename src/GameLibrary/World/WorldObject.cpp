
#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform3DAspect.hpp>

namespace fgl
{
	WorldObject::WorldObject()
		: world(nullptr) {
		//
	}
	
	bool WorldObject::getFlag(const String& flag) const {
		if(flag == "WorldObject") {
			return true;
		}
		return false;
	}
	
	String WorldObject::getDebugDescription() const {
		auto transform3d = getAspect<Transform3DAspect>();
		if(transform3d != nullptr) {
			return "WorldObject @ "+transform3d->getPosition().toString();
		}
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d != nullptr) {
			return "WorldObject @ "+transform2d->getPosition().toString();
		}
		return "WorldObject";
	}
	
	World* WorldObject::getWorld() {
		return world;
	}
	
	const World* WorldObject::getWorld() const {
		return world;
	}
	
	WorldObject* WorldObject::addAspect(WorldObjectAspect* aspect) {
		if(aspect->worldObject != nullptr) {
			throw IllegalArgumentException("aspect", "already added to an object");
		}
		Aspectable::addAspect(aspect);
		return this;
	}
	
	void WorldObject::update(const ApplicationData& appData) {
		for(auto aspect : getAllAspects()) {
			aspect->update(appData);
		}
	}
	
	void WorldObject::onAddAspect(WorldObjectAspect* aspect) {
		aspect->worldObject = this;
		aspect->onAddToObject(this);
		if(world != nullptr) {
			aspect->onAddObjectToWorld(world);
		}
	}
	
	void WorldObject::onAddToWorld(World* world) {
		// open for implementation
	}
	
	void WorldObject::onRemoveFromWorld(World* world) {
		// open for implementation
	}
	
	
	
	
	WorldObjectAspect::WorldObjectAspect()
		: worldObject(nullptr) {
		//
	}
	
	bool WorldObjectAspect::getFlag(const String& flag) const {
		if(flag == "WorldObjectAspect") {
			return true;
		}
		return false;
	}
	
	void WorldObjectAspect::update(const ApplicationData& appData) {
		// open for implementation
	}
	
	WorldObject* WorldObjectAspect::getObject() {
		return worldObject;
	}
	
	const WorldObject* WorldObjectAspect::getObject() const {
		return worldObject;
	}
	
	World* WorldObjectAspect::getWorld() {
		if(worldObject == nullptr) {
			return nullptr;
		}
		return worldObject->world;
	}
	
	const World* WorldObjectAspect::getWorld() const {
		if(worldObject == nullptr) {
			return nullptr;
		}
		return worldObject->world;
	}
	
	void WorldObjectAspect::onAddToObject(WorldObject* object) {
		// open for implementation
	}
	
	void WorldObjectAspect::onAddObjectToWorld(World* world) {
		// open for implementation
	}
	
	void WorldObjectAspect::onRemoveObjectFromWorld(World* world) {
		// open for implementation
	}
}
