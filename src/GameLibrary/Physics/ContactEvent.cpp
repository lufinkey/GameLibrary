
#include <GameLibrary/Physics/ContactEvent.hpp>

namespace fgl
{
	const EventType EVENT_CONTACT = registerEventType();

	ContactEvent::ContactEvent(Collidable* contacted, ContactState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs)
		: contacted(contacted),
		state(state),
		rectTagPairs(rectTagPairs),
		prevRectTagPairs(prevRectTagPairs)
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
}
