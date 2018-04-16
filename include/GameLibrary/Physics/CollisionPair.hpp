
#pragma once

#include "Collidable.hpp"
#include "CollisionRects/CollisionRect.hpp"

namespace fgl
{
	class Collidable;

	typedef std::pair<CollisionRect*, CollisionRect*> CollisionRectPair;
	typedef std::pair<String, String> CollisionRectTagPair;

	class CollisionPair
	{
	public:
		Collidable* collidable1;
		Collidable* collidable2;
		//! the rect pairs that had a collision in the previous frame
		ArrayList<CollisionRectTagPair> priorityRects;
		//! the sides of collidable1 that were collided on
		ArrayList<CollisionSide> sides;
		//! the rect pairs that should be ignored in the next frame if they have a collision
		ArrayList<CollisionRectTagPair> ignoredCollisions;

		CollisionPair(Collidable* collidable1, Collidable* collidable2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		bool shouldIgnoreCollision(CollisionRect* rect1, CollisionRect* rect2) const;

		ArrayList<CollisionRectPair> getCollisionRectPairs(const ArrayList<CollisionRect*>& rects1, const ArrayList<CollisionRect*>& rects2) const;

		bool isContacting() const;
	};
}
