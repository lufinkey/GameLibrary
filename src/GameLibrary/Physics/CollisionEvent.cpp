
#include <GameLibrary/Physics/CollisionEvent.hpp>

namespace fgl
{
	CollisionSide CollisionSide_getOpposite(CollisionSide side)
	{
		switch(side)
		{
			case COLLISIONSIDE_LEFT:
				return COLLISIONSIDE_RIGHT;
				
			case COLLISIONSIDE_RIGHT:
				return COLLISIONSIDE_LEFT;
				
			case COLLISIONSIDE_TOP:
				return COLLISIONSIDE_BOTTOM;
				
			case COLLISIONSIDE_BOTTOM:
				return COLLISIONSIDE_TOP;
		}
		throw IllegalArgumentException("side", "invalid CollisionSide enum value");
	}
	
	const EventType EVENT_COLLISION = registerEventType();

	CollisionEvent::CollisionEvent(Collidable* collided, CollisionSide side, CollisionState state)
		: collided(collided),
		side(side),
		state(state)
	{
		//
	}

	EventType CollisionEvent::getEventType() const
	{
		return EVENT_COLLISION;
	}
	
	Collidable* CollisionEvent::getCollided() const
	{
		return collided;
	}
	
	CollisionSide CollisionEvent::getCollisionSide() const
	{
		return side;
	}
	
	CollisionState CollisionEvent::getCollisionState() const
	{
		return state;
	}
}
