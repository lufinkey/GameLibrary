
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"
#include <list>

namespace fgl
{
	class CollisionManagerEventListener;
	
	
	
	class CollisionManager
	{
	public:
		CollisionManager();
		virtual ~CollisionManager() = default;

		virtual void addCollidable(Collidable* collidable);
		virtual void removeCollidable(Collidable* collidable);
		
		void addEventListener(CollisionManagerEventListener* eventListener);
		void removeEventListener(CollisionManagerEventListener* eventListener);

		virtual void update(const ApplicationData& appData);
		
		const std::list<Collidable*>& getCollidables() const;
		
	protected:
		struct UpdateData
		{
			std::list<std::function<void()>> onCollisionCalls;
			std::list<std::function<void()>> onCollisionFinishCalls;
			std::list<std::function<void()>> onContactCalls;
			std::list<std::function<void()>> onContactFinishCalls;
		};
		
		virtual void onWillBeginCollisionUpdates(const ApplicationData& appData);
		virtual void onBeginCollisionUpdates(const ApplicationData& appData);
		
		virtual bool respondsToCollision(const ApplicationData& appData, const Collidable* collidable1, const Collidable* collidable2, CollisionRectPair rectPair, CollisionSide side) const;
		virtual void dispatchContactEvents(const ApplicationData& appData, ContactState state, const CollisionPair& pair, const CollisionPair& prevPair);
		virtual void dispatchCollisionEvents(const ApplicationData& appData, CollisionState state, CollisionSide side, const CollisionPair& pair, const CollisionPair& prevPair);
		
		virtual void performFinalCollisionUpdates(const ApplicationData& appData, const CollisionPair& pair, const CollisionPair& prevPair, UpdateData& updateData);
		
		virtual void onWillFinishCollisionUpdates(const ApplicationData& appData, UpdateData& updateData);
		virtual void onFinishCollisionUpdates(const ApplicationData& appData);
		
		const std::list<CollisionPair>& getRemovedCollisions() const;

	private:
		CollisionSide getCollisionSide(const Vector2d& shiftAmount) const;
		std::list<CollisionPair> getCollisionPairs() const;

		std::list<Collidable*> collidables;
		std::list<CollisionPair> previousCollisions;
		
		std::list<CollisionPair> removedCollisions;
		
		std::list<CollisionManagerEventListener*> listeners;
	};
	
	
	
	class CollisionManagerEventListener
	{
		friend class CollisionManager;
	public:
		virtual ~CollisionManagerEventListener() = default;
		
	protected:
		virtual void onBeginCollisionUpdates(CollisionManager* collisionManager, const ApplicationData& appData);
		virtual void onFinishCollisionUpdates(CollisionManager* collisionManager, const ApplicationData& appData);
	};
}
