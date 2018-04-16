
#pragma once

#include <memory>
#include <GameLibrary/GameLibrary.hpp>

namespace fgl
{
	typedef fgl::Uint32 EventType;
	EventType registerEventType();
	
	class Event
	{
	public:
		virtual ~Event();
		virtual EventType getEventType() const = 0;
	};
	
	typedef std::shared_ptr<Event> EventPtr;
}
