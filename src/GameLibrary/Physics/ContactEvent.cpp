
#include <GameLibrary/Physics/ContactEvent.hpp>

namespace fgl
{
	const EventType EVENT_CONTACT = registerEventType();

	ContactEvent::ContactEvent(Collidable* target, Collidable* contacted, ContactState state,
		const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs,
		const ArrayList<CollisionRectTagPair>& ignoredRectPairs,
		const ArrayList<CollisionSide>& collidingSides,
		const ApplicationData* appData)
		: target(target),
		contacted(contacted),
		state(state),
		rectTagPairs(rectTagPairs),
		prevRectTagPairs(prevRectTagPairs),
		ignoredRectPairs(ignoredRectPairs),
		collidingSides(collidingSides),
		appData(appData)
	{
		//
	}

	EventType ContactEvent::getEventType() const
	{
		return EVENT_CONTACT;
	}
	
	Collidable* ContactEvent::getTarget() const
	{
		return target;
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
	
	const ArrayList<CollisionRectTagPair>& ContactEvent::getIgnoredRectPairs() const
	{
		return ignoredRectPairs;
	}
	
	const ArrayList<CollisionSide>& ContactEvent::getCollidingSides() const
	{
		return collidingSides;
	}
	
	const ApplicationData* ContactEvent::getAppData() const {
		return appData;
	}
}
