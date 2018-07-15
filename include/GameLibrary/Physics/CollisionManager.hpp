
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"
#include <list>

namespace fgl
{
	class CollisionManager
	{
	public:
		CollisionManager();
		virtual ~CollisionManager() = default;

		virtual void addCollidable(Collidable* collidable);
		virtual void removeCollidable(Collidable* collidable);

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
		
		virtual bool respondsToCollision(Collidable* collidable1, Collidable* collidable2, CollisionRectPair rectPair, CollisionSide side) const;
		virtual void dispatchContactEvents(ContactState state, const CollisionPair& pair, const CollisionPair& prevPair);
		virtual void dispatchCollisionEvents(CollisionState state, CollisionSide side, const CollisionPair& pair, const CollisionPair& prevPair);
		
		virtual void performFinalCollisionUpdates(const CollisionPair& pair, const CollisionPair& prevPair, UpdateData& updateData);
		
		virtual void onWillFinishCollisionUpdates(UpdateData& updateData);
		virtual void onFinishCollisionUpdates();

	private:
		CollisionSide getCollisionSide(const Vector2d& shiftAmount) const;
		std::list<CollisionPair> getCollisionPairs() const;

		std::list<Collidable*> collidables;
		std::list<CollisionPair> previousCollisions;
	};
}

REGISTER_TYPE(fgl::CollisionManager)
