
#include <GameLibrary/Event/Event.hpp>

namespace fgl
{
	Event::~Event()
	{
		//
	}
	
	EventType registerEventType()
	{
		static EventType nextType = 1;
		EventType eventType = nextType;
		nextType++;
		return eventType;
	}
}
