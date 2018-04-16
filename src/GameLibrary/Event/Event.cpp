
#include <fightlib/base/Event.hpp>

namespace fl
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
