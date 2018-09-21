
#pragma once

#include <GameLibrary/Draw/DrawManager.hpp>
#include <GameLibrary/Physics/CollisionManager.hpp>
#include <GameLibrary/Window/AssetManager.hpp>
#include "WorldObject.hpp"
#include "WorldCamera.hpp"
#include <list>

namespace fgl
{
	class World
	{
		friend class WorldCamera;
	public:
		World(AssetManager* assetManager, const ArrayList<WorldCamera*>& cameras = {});
		virtual ~World();
		
		virtual void update(ApplicationData appData);
		virtual void draw(ApplicationData appData, Graphics graphics) const;
		
		DrawManager* getDrawManager();
		const DrawManager* getDrawManager() const;
		CollisionManager* getCollisionManager();
		const CollisionManager* getCollisionManager() const;
		
		AssetManager* getAssetManager();
		const AssetManager* getAssetManager() const;
		
		void addDrawable(Drawable* drawable, const std::function<void(Graphics&)>& filter=nullptr);
		void addDrawables(const ArrayList<Drawable*>& drawables, const std::function<void(Graphics&)>& filter=nullptr);
		void removeDrawable(Drawable* drawable);
		void removeDrawables(const ArrayList<Drawable*>& drawables);
		
		void addCollidable(Collidable* collidable);
		void addCollidables(const ArrayList<Collidable*>& collidables);
		void removeCollidable(Collidable* collidable);
		void removeCollidables(const ArrayList<Collidable*>& collidables);
		
		void addObject(WorldObject* object);
		void removeObject(WorldObject* object);
		void destroyObject(WorldObject* object);
		
		const std::list<WorldObject*>& getObjects(); 
		const std::list<const WorldObject*>& getObjects() const;
		std::list<WorldObject*> getObjectsWhere(const std::function<bool(const WorldObject*)>& func);
		std::list<const WorldObject*> getObjectsWhere(const std::function<bool(const WorldObject*)>& func) const;
		WorldObject* getObjectWhere(const std::function<bool(const WorldObject*)>& func);
		const WorldObject* getObjectWhere(const std::function<bool(const WorldObject*)>& func) const;
		
		void setGravity(const Vector2d& velocity);
		Vector2d getGravity() const;
		
		const ArrayList<WorldCamera*>& getCameras();
		const ArrayList<const WorldCamera*>& getCameras() const;
		
		void runBeforeUpdate(std::function<void()> func);
		void runAfterUpdate(std::function<void()> func);
		
	protected:
		World(DrawManager* drawManager, CollisionManager* collisionManager, AssetManager* assetManager, const ArrayList<WorldCamera*>& cameras = {});
		
	private:
		void addObjectAspects(WorldObject* object);
		void removeObjectAspects(WorldObject* object);
		
		Screen* screen;
		DrawManager* drawManager;
		CollisionManager* collisionManager;
		std::list<WorldObject*> objects;
		Vector2d gravity;
		ArrayList<WorldCamera*> cameras;
		AssetManager* assetManager;
		
		std::list<std::function<void()>> preUpdateQueue;
		std::list<std::function<void()>> postUpdateQueue;
	};
}
