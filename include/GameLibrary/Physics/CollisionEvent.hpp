
#pragma once

#include <GameLibrary/Event/Event.hpp>
#include "CollisionRectPair.hpp"
#include "CollisionSide.hpp"

namespace fgl
{
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
		CollisionEvent(Collidable* target, Collidable* collided, CollisionSide side, CollisionState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs);
		
		virtual EventType getEventType() const override;
		
		Collidable* getTarget() const;
		Collidable* getCollided() const;
		CollisionSide getCollisionSide() const;
		CollisionState getCollisionState() const;
		const ArrayList<CollisionRectTagPair>& getRectTagPairs() const;
		const ArrayList<CollisionRectTagPair>& getPreviousRectTagPairs() const;
		
	private:
		Collidable* target;
		Collidable* collided;
		CollisionSide side;
		CollisionState state;
		ArrayList<CollisionRectTagPair> rectTagPairs;
		ArrayList<CollisionRectTagPair> prevRectTagPairs;
	};
}

REGISTER_TYPE(fgl::CollisionEvent, fgl::Event)
