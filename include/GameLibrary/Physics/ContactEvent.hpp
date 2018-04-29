
#pragma once

#include <GameLibrary/Event/Event.hpp>
#include "CollisionRectPair.hpp"

namespace fgl
{
	typedef enum
	{
		CONTACTSTATE_NEW,
		CONTACTSTATE_UPDATED,
		CONTACTSTATE_FINISHED
	} ContactState;

	extern const EventType EVENT_CONTACT;

	class Collidable;

	class ContactEvent : public Event
	{
	public:
		ContactEvent(Collidable* contacted, ContactState state, const ArrayList<CollisionRectTagPair>& rectTagPairs, const ArrayList<CollisionRectTagPair>& prevRectTagPairs);

		virtual EventType getEventType() const override;

		Collidable* getContacted() const;
		ContactState getContactState() const;
		const ArrayList<CollisionRectTagPair>& getRectTagPairs() const;
		const ArrayList<CollisionRectTagPair>& getPreviousRectTagPairs() const;

	private:
		Collidable* contacted;
		ContactState state;
		ArrayList<CollisionRectTagPair> rectTagPairs;
		ArrayList<CollisionRectTagPair> prevRectTagPairs;
	};
}
