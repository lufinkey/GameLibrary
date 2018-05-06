
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

		void addCollidable(Collidable* collidable);
		void removeCollidable(Collidable* collidable);

		virtual void update(const ApplicationData& appData);
		
		const std::list<Collidable*>& getCollidables() const;
		
	protected:
		virtual bool respondsToCollision(Collidable* collidable1, Collidable* collidable2, CollisionRectPair rectPair, CollisionSide side) const;
		virtual void dispatchContactEvents(ContactState state, const CollisionPair& pair, const CollisionPair& prevPair);
		virtual void dispatchCollisionEvents(CollisionState state, CollisionSide side, const CollisionPair& pair, const CollisionPair& prevPair);

	private:
		CollisionSide getCollisionSide(const Vector2d& shiftAmount) const;
		std::list<CollisionPair> getCollisionPairs() const;

		std::list<Collidable*> collidables;
		std::list<CollisionPair> previousCollisions;
	};
}
