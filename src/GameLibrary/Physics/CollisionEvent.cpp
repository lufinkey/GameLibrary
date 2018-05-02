
#include <GameLibrary/Physics/CollisionEvent.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	const EventType EVENT_COLLISION = registerEventType();

	CollisionEvent::CollisionEvent(Collidable* collided, CollisionSide side, CollisionState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs)
		: collided(collided),
		side(side),
		state(state),
		rectTagPairs(rectTagPairs),
		prevRectTagPairs(prevRectTagPairs)
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
	
	const ArrayList<CollisionRectTagPair>& CollisionEvent::getRectTagPairs() const
	{
		return rectTagPairs;
	}
	
	const ArrayList<CollisionRectTagPair>& CollisionEvent::getPreviousRectTagPairs() const
	{
		return prevRectTagPairs;
	}
}
