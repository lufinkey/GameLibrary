
#pragma once

#include "Collidable.hpp"
#include "CollisionPair.hpp"

namespace fgl
{
	class CollisionManager
	{
	public:
		CollisionManager();

		void addCollidable(Collidable* collidable);
		void removeCollidable(Collidable* collidable);

		void update(const ApplicationData& appData);
		
		const ArrayList<Collidable*>& getCollidables() const;

	private:
		CollisionSide getCollisionSide(const Vector2d& shiftAmount) const;
		ArrayList<CollisionPair> getCollisionPairs() const;

		ArrayList<Collidable*> collidables;
		ArrayList<CollisionPair> previousCollisions;
	};
}
