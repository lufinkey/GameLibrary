
#pragma once

#include "Collidable.hpp"
#include "CollisionRects/CollisionRect.hpp"

namespace fgl
{
	class Collidable;
	
	class CollisionPair
	{
	public:
		Collidable* collidable1;
		Collidable* collidable2;
		//! the rect pairs that had a collision in the previous frame
		std::list<CollisionRectTagPair> collidedRectPairs;
		//! the sides of collidable1 that were collided on
		std::list<CollisionSide> sides;
		//! the rect pairs that should be ignored in the next frame if they have a collision
		std::list<CollisionRectTagPair> ignoredRectPairs;

		CollisionPair(Collidable* collidable1, Collidable* collidable2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		bool shouldIgnoreCollision(const CollisionRect* rect1, const CollisionRect* rect2) const;
		
		// get the rect tag pairs that are contacting each other
		std::list<CollisionRectTagPair> getContactingRectPairs() const;
		std::list<CollisionRectTagPair> getReverseContactingRectPairs() const;

		std::list<CollisionRectPair> getCollisionRectPairs(const ArrayList<const CollisionRect*>& rects1, const ArrayList<const CollisionRect*>& rects2) const;
		// returns collidedRectPairs where first is collidable2 and second is collidable1
		std::list<CollisionRectTagPair> getReverseCollidedRectPairs() const;
		// returns the collision sides with respect to collidable2
		std::list<CollisionSide> getOppositeSides() const;
		// returns ignoredRectPairs where first is collidable2 and second is collidable1
		std::list<CollisionRectTagPair> getReverseIgnoredRectPairs() const;

		bool isContacting() const;
	};
}
