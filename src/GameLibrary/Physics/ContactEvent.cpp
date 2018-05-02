
#include <GameLibrary/Physics/ContactEvent.hpp>

namespace fgl
{
	const EventType EVENT_CONTACT = registerEventType();

	ContactEvent::ContactEvent(Collidable* contacted, ContactState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs, const ArrayList<CollisionSide>& collidingSides)
		: contacted(contacted),
		state(state),
		rectTagPairs(rectTagPairs),
		prevRectTagPairs(prevRectTagPairs),
		collidingSides(collidingSides)
	{
		//
	}

	EventType ContactEvent::getEventType() const
	{
		return EVENT_CONTACT;
	}

	Collidable* ContactEvent::getContacted() const
	{
		return contacted;
	}

	ContactState ContactEvent::getContactState() const
	{
		return state;
	}
	
	const ArrayList<CollisionRectTagPair>& ContactEvent::getRectTagPairs() const
	{
		return rectTagPairs;
	}
	
	const ArrayList<CollisionRectTagPair>& ContactEvent::getPreviousRectTagPairs() const
	{
		return prevRectTagPairs;
	}
	
	const ArrayList<CollisionSide>& ContactEvent::getCollidingSides() const
	{
		return collidingSides;
	}
}
