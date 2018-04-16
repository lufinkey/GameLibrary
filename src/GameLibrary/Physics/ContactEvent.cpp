
#include <GameLibrary/Physics/ContactEvent.hpp>

namespace fgl
{
	const EventType EVENT_CONTACT = registerEventType();

	ContactEvent::ContactEvent(Collidable* contacted, ContactState state)
		: contacted(contacted),
		state(state)
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
}
