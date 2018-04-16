
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"

namespace fgl
{
	class CollisionManager
	{
	public:
		CollisionManager();

		void addCollidable(Collidable* entity);
		void removeCollidable(Collidable* entity);

		void update(const ApplicationData& appData);

	private:
		CollisionSide getCollisionSide(const Vector2d& shiftAmount) const;

		ArrayList<Collidable*> collidables;
		ArrayList<CollisionPair> previousCollisions;

		ArrayList<CollisionPair> getCollisionPairs() const;
	};
}
