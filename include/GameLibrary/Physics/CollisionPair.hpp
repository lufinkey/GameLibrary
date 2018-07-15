
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
		ArrayList<CollisionRectTagPair> collidedRectPairs;
		//! the sides of collidable1 that were collided on
		ArrayList<CollisionSide> sides;
		//! the rect pairs that should be ignored in the next frame if they have a collision
		ArrayList<CollisionRectTagPair> ignoredRectPairs;

		CollisionPair(Collidable* collidable1, Collidable* collidable2);

		bool operator==(const CollisionPair& pair) const;
		bool operator!=(const CollisionPair& pair) const;

		bool shouldIgnoreCollision(CollisionRect* rect1, CollisionRect* rect2) const;
		
		// get the rect tag pairs that are contacting each other
		ArrayList<CollisionRectTagPair> getContactingRectPairs() const;
		ArrayList<CollisionRectTagPair> getReverseContactingRectPairs() const;

		ArrayList<CollisionRectPair> getCollisionRectPairs(const ArrayList<CollisionRect*>& rects1, const ArrayList<CollisionRect*>& rects2) const;
		// returns collidedRectPairs where first is collidable2 and second is collidable1
		ArrayList<CollisionRectTagPair> getReverseCollidedRectPairs() const;
		// returns the collision sides with respect to collidable2
		ArrayList<CollisionSide> getOppositeSides() const;
		// returns ignoredRectPairs where first is collidable2 and second is collidable1
		ArrayList<CollisionRectTagPair> getReverseIgnoredRectPairs() const;

		bool isContacting() const;
	};
}

REGISTER_TYPE(fgl, CollisionPair)
