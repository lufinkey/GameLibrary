
#pragma once

#include <functional>
#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>

struct SDL_Window;

namespace fgl
{
	class Application;
	
	
	/*! Manages Application and Window events. Automatically called in the Application loop.*/
	class EventManager
	{
		friend class Keyboard;
		friend class Mouse;
		friend class Multitouch;
		friend class Window;
	private:
		static const ArrayList<Window*>& getWindows();
		static void* getWindowMutexPtr();
		static Window* getWindowFromID(unsigned int windowID);
		static SDL_Window* getSDLWindow(Window*window);
		
		static void addWindow(Window*window);
		static void removeWindow(Window*window);
		
	public:
		/*! Polls all queued events*/
		static void update(bool updateInputs=true);
		static bool recievedQuitRequest();
	};
}
