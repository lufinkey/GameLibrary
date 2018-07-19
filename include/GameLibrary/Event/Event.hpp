
#pragma once

#include <memory>
#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/String.hpp>

namespace fgl
{
	typedef Uint32 EventType;
	EventType registerEventType();
	
	class Event
	{
	public:
		virtual ~Event();
		virtual EventType getEventType() const = 0;
	};
	
	typedef std::shared_ptr<Event> EventPtr;
}
