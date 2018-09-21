
#pragma once

#include <GameLibrary/Utilities/Aspectable.hpp>
#include <GameLibrary/Utilities/String.hpp>

namespace fgl
{
	class World;
	class WorldObjectAspect;
	
	
	
	class WorldObject : public Aspectable<WorldObjectAspect>
	{
		friend class World;
		friend class WorldObjectAspect;
	public:
		WorldObject();
		virtual ~WorldObject() = default;
		
		virtual String getDebugDescription() const;
		virtual bool getFlag(const String& flag) const;
		
		virtual void update(const ApplicationData& appData);
		
		WorldObject* addAspect(WorldObjectAspect* aspect);
		
		World* getWorld();
		const World* getWorld() const;
		
	protected:
		virtual void onAddAspect(WorldObjectAspect* aspect);
		
		virtual void onAddToWorld(World* world);
		virtual void onRemoveFromWorld(World* world);
		
	private:
		World* world;
	};
	
	
	
	class WorldObjectAspect
	{
		friend class World;
		friend class WorldObject;
	public:
		WorldObjectAspect();
		
		virtual bool getFlag(const String& flag) const;
		virtual void update(const ApplicationData& appData);
		
		WorldObject* getObject();
		const WorldObject* getObject() const;
		
		World* getWorld();
		const World* getWorld() const;
		
		template<typename CLASS>
		inline CLASS* getAspect() {
			if(worldObject == nullptr) {
				return nullptr;
			}
			return worldObject->getAspect<CLASS>();
		}
		
		template<typename CLASS>
		inline const CLASS* getAspect() const {
			if(worldObject == nullptr) {
				return nullptr;
			}
			return worldObject->getAspect<CLASS>();
		}
		
		template<typename CLASS>
		inline CLASS* getAspectWhere(const std::function<bool(const CLASS*)>& func) {
			if(worldObject == nullptr) {
				return nullptr;
			}
			return worldObject->getAspectWhere<CLASS>(func);
		}
		
		template<typename CLASS>
		inline const CLASS* getAspectWhere(const std::function<bool(const CLASS*)>& func) const {
			if(worldObject == nullptr) {
				return nullptr;
			}
			return worldObject->getAspectWhere<CLASS>(func);
		}
		
		template<typename CLASS>
		inline std::list<CLASS*> getAspects() {
			if(worldObject == nullptr) {
				return {};
			}
			return worldObject->getAspects<CLASS>();
		}
		
		template<typename CLASS>
		inline std::list<const CLASS*> getAspects() const {
			if(worldObject == nullptr) {
				return {};
			}
			return reinterpret_cast<const WorldObject*>(worldObject)->getAspects<CLASS>();
		}
		
	protected:
		virtual void onAddToObject(WorldObject* object);
		
		virtual void onAddObjectToWorld(World* world);
		virtual void onRemoveObjectFromWorld(World* world);
		
	private:
		WorldObject* worldObject;
	};
}
