
#include <GameLibrary/World/World.hpp>
#include <GameLibrary/Screen/Screen.hpp>
#include <GameLibrary/Utilities/PerformanceMacros.hpp>
#include <chrono>

namespace fgl
{
	//#define DEBUG_DEPTH
	//#define DEBUG_TIME
	
	World::World(AssetManager* assetManager, const ArrayList<WorldCamera*>& cameras)
		: World(nullptr, nullptr, assetManager, cameras) {
		
	}
	
	World::World(DrawManager* drawManager, CollisionManager* collisionManager, AssetManager* assetManager, const ArrayList<WorldCamera*>& cameras)
		: screen(nullptr),
		drawManager(drawManager ? drawManager : new DrawManager()),
		collisionManager(collisionManager ? collisionManager : new CollisionManager()),
		cameras(cameras),
		assetManager(assetManager) {
		for(auto camera : cameras) {
			if(camera->world != nullptr) {
				throw IllegalArgumentException("cameras", "camera cannot be used in multiple worlds");
			}
		}
		for(auto camera : cameras) {
			camera->world = this;
		}
	}

	World::~World() {
		for(auto object : objects) {
			delete object;
		}
		if(screen != nullptr) {
			delete screen;
			screen = nullptr;
		}
		for(auto camera : cameras) {
			delete camera;
		}
	}
	
	void World::update(ApplicationData appData) {
		auto nextPreUpdateQueue = std::list<std::function<void()>>();
		nextPreUpdateQueue.swap(preUpdateQueue);
		preUpdateQueue.clear();
		for(auto& func : nextPreUpdateQueue) {
			func();
		}
		
		auto window = appData.getWindow();
		if(screen == nullptr) {
			screen = new Screen(window);
			auto screenElement = screen->getElement();
			for(auto camera : cameras) {
				screenElement->addChildElement(camera->getScreenElement());
			}
		}
		
		
		#ifdef DEBUG_TIME
		START_PERFORMANCE_LOG(objectUpdates)
		#endif
		
		// set extra appData
		appData.additionalData["world"] = this;
		// update objects
		for(auto object : objects) {
			object->update(appData);
		}
		
		#ifdef DEBUG_TIME
		FINISH_PERFORMANCE_LOG_ROUNDED(objectUpdates, "object updates")
		#endif
		
		
		// update collision manager
		#ifdef DEBUG_TIME
		START_PERFORMANCE_LOG(collisions)
		#endif
		
		collisionManager->update(appData);
		
		#ifdef DEBUG_TIME
		FINISH_PERFORMANCE_LOG_ROUNDED(collisions, "collision updates")
		#endif
		
		
		// update draw manager
		#ifdef DEBUG_TIME
		START_PERFORMANCE_LOG(drawOrdering)
		#endif
		
		drawManager->update(appData);
		
		#ifdef DEBUG_TIME
		FINISH_PERFORMANCE_LOG_ROUNDED(drawOrdering, "draw ordering")
		#endif
		
		// update overlay screen
		screen->update(appData);
		
		auto nextPostUpdateQueue = std::list<std::function<void()>>();
		nextPostUpdateQueue.swap(postUpdateQueue);
		postUpdateQueue.clear();
		for(auto& func : nextPostUpdateQueue) {
			func();
		}
	}
	
	void World::draw(ApplicationData appData, Graphics graphics) const {
		#ifdef DEBUG_TIME
		START_PERFORMANCE_LOG(drawing)
		#endif
		
		// set extra appData
		appData.additionalData["world"] = this;
		// draw
		if(cameras.size() == 0) {
			auto viewSize = appData.getWindow()->getViewport()->getSize();
			graphics.translate(viewSize / 2.0);
			drawManager->draw(DrawContext(&appData, nullptr, drawManager), graphics);
		}
		else if(screen != nullptr) {
			screen->draw(appData, graphics);
		}
		
		#ifdef DEBUG_TIME
		FINISH_PERFORMANCE_LOG_ROUNDED(drawing, "draw")
		#endif
	}
	
	DrawManager* World::getDrawManager() {
		return drawManager;
	}
	
	const DrawManager* World::getDrawManager() const {
		return drawManager;
	}
	
	CollisionManager* World::getCollisionManager() {
		return collisionManager;
	}
	
	const CollisionManager* World::getCollisionManager() const {
		return collisionManager;
	}
	
	AssetManager* World::getAssetManager() {
		return assetManager;
	}
	
	const AssetManager* World::getAssetManager() const {
		return assetManager;
	}
	
	void World::addDrawable(Drawable* drawable, const std::function<void(Graphics&)>& filter) {
		drawManager->addDrawable(drawable, filter);
	}
	
	void World::addDrawables(const ArrayList<Drawable*>& drawables, const std::function<void(Graphics&)>& filter) {
		for(auto& drawable : drawables) {
			drawManager->addDrawable(drawable, filter);
		}
	}
	
	void World::removeDrawable(Drawable* drawable) {
		drawManager->removeDrawable(drawable);
	}
	
	void World::removeDrawables(const ArrayList<Drawable*>& drawables) {
		for(auto& drawable : drawables) {
			drawManager->removeDrawable(drawable);
		}
	}
	
	void World::addCollidable(Collidable* collidable) {
		collisionManager->addCollidable(collidable);
	}
	
	void World::addCollidables(const ArrayList<Collidable*>& collidables) {
		for(auto& collidable : collidables) {
			collisionManager->addCollidable(collidable);
		}
	}
	
	void World::removeCollidable(Collidable* collidable) {
		collisionManager->removeCollidable(collidable);
	}
	
	void World::removeCollidables(const ArrayList<Collidable*>& collidables) {
		for(auto& collidable : collidables) {
			collisionManager->removeCollidable(collidable);
		}
	}
	
	void World::addObject(WorldObject* object) {
		if(object->world != nullptr) {
			throw IllegalArgumentException("object", "already added to a world");
		}
		objects.push_back(object);
		object->world = this;
		object->onAddToWorld(this);
		for(auto aspect : object->getAllAspects()) {
			aspect->onAddObjectToWorld(this);
		}
	}
	
	void World::removeObject(WorldObject* object) {
		if(object->world == nullptr || object->world != this) {
			return;
		}
		auto objectIt = std::find(objects.begin(), objects.end(), object);
		objects.erase(objectIt);
		object->world = nullptr;
		object->onRemoveFromWorld(this);
		for(auto aspect : object->getAllAspects()) {
			aspect->onRemoveObjectFromWorld(this);
		}
	}
	
	void World::destroyObject(WorldObject* object) {
		if(object->world != nullptr) {
			throw IllegalArgumentException("object", "must be removed from its world before it is destroyed");
		}
		runAfterUpdate([=]() {
			runAfterUpdate([=]() {
				delete object;
			});
		});
	}
	
	const std::list<WorldObject*>& World::getObjects() {
		return objects;
	}
	
	const std::list<const WorldObject*>& World::getObjects() const {
		return reinterpret_cast<const std::list<const WorldObject*>&>(objects);
	}
	
	std::list<WorldObject*> World::getObjectsWhere(const std::function<bool(const WorldObject*)>& func) {
		std::list<WorldObject*> matchingObjects;
		for(auto object : objects) {
			if(func(object)) {
				matchingObjects.push_back(object);
			}
		}
		return matchingObjects;
	}
	
	std::list<const WorldObject*> World::getObjectsWhere(const std::function<bool(const WorldObject*)>& func) const {
		std::list<const WorldObject*> matchingObjects;
		for(auto object : objects) {
			if(func(object)) {
				matchingObjects.push_back(object);
			}
		}
		return matchingObjects;
	}
	
	WorldObject* World::getObjectWhere(const std::function<bool(const WorldObject*)>& func) {
		for(auto object : objects) {
			if(func(object)) {
				return object;
			}
		}
		return nullptr;
	}
	
	const WorldObject* World::getObjectWhere(const std::function<bool(const WorldObject*)>& func) const {
		for(auto object : objects) {
			if(func(object)) {
				return object;
			}
		}
		return nullptr;
	}
	
	void World::setGravity(const Vector2d& gravity_arg) {
		gravity = gravity_arg;
	}
	
	Vector2d World::getGravity() const {
		return gravity;
	}
	
	const ArrayList<WorldCamera*>& World::getCameras() {
		return cameras;
	}
	
	const ArrayList<const WorldCamera*>& World::getCameras() const {
		return reinterpret_cast<const ArrayList<const WorldCamera*>&>(cameras);
	}
	
	void World::runBeforeUpdate(std::function<void()> func) {
		preUpdateQueue.push_back(func);
	}
	
	void World::runAfterUpdate(std::function<void()> func) {
		postUpdateQueue.push_back(func);
	}
}
