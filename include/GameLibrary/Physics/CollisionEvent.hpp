
#pragma once

#include <GameLibrary/Event/Event.hpp>
#include "CollisionRectPair.hpp"

namespace fgl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;
	
	CollisionSide CollisionSide_getOpposite(CollisionSide side);
	
	typedef enum
	{
		COLLISIONSTATE_NEW,
		COLLISIONSTATE_UPDATED,
		COLLISIONSTATE_FINISHED
	} CollisionState;

	extern const EventType EVENT_COLLISION;
	
	class Collidable;
	
	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(Collidable* collided, CollisionSide side, CollisionState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs);
		
		virtual EventType getEventType() const override;
		
		Collidable* getCollided() const;
		CollisionSide getCollisionSide() const;
		CollisionState getCollisionState() const;
		const ArrayList<CollisionRectTagPair>& getRectTagPairs() const;
		const ArrayList<CollisionRectTagPair>& getPreviousRectTagPairs() const;
		
	private:
		Collidable* collided;
		CollisionSide side;
		CollisionState state;
		ArrayList<CollisionRectTagPair> rectTagPairs;
		ArrayList<CollisionRectTagPair> prevRectTagPairs;
	};
}
