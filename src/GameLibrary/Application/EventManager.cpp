
#include "EventManager.hpp"
#include <GameLibrary/Application/Application.hpp>
#include <GameLibrary/Input/Keyboard.hpp>
#include <GameLibrary/Input/Mouse.hpp>
#include <GameLibrary/Input/Multitouch.hpp>
#include <GameLibrary/Utilities/Thread.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <SDL.h>
#include <condition_variable>

namespace fgl
{
	ArrayList<Window*> EventManager_windows;
	static std::mutex EventManager_windows_mutex;
	static bool EventManager_quitRequest = false;
	
	Keyboard::Key Keyboard_SDLK_to_Key(int code);
	Mouse::Button Mouse_SDL_to_MouseButton(byte button);
	
	const ArrayList<Window*>& EventManager::getWindows()
	{
		return EventManager_windows;
	}
	
	void*EventManager::getWindowMutexPtr()
	{
		return (void*)(&EventManager_windows_mutex);
	}
	
	Window* EventManager::getWindowFromID(unsigned int windowID)
	{
		EventManager_windows_mutex.lock();
		for(unsigned int i=0; i<EventManager_windows.size(); i++)
		{
			Window* window = EventManager_windows.get(i);
			if(window->windowID == windowID)
			{
				EventManager_windows_mutex.unlock();
				return window;
			}
		}
		EventManager_windows_mutex.unlock();
		return nullptr;
	}
	
	SDL_Window* EventManager::getSDLWindow(Window*window)
	{
		if(window == nullptr)
		{
			return nullptr;
		}
		return window->sdlWindow;
	}
	
	void EventManager::addWindow(Window*window)
	{
		EventManager_windows_mutex.lock();
		EventManager_windows.add(window);
		EventManager_windows_mutex.unlock();
	}
	
	void EventManager::removeWindow(Window*window)
	{
		EventManager_windows_mutex.lock();
		for(unsigned int i=0; i<EventManager_windows.size(); i++)
		{
			if(EventManager_windows.get(i) == window)
			{
				EventManager_windows.remove(i);
				EventManager_windows_mutex.unlock();
				return;
			}
		}
		EventManager_windows_mutex.unlock();
		Mouse::removeWindow(window);
		Multitouch::removeWindow(window);
	}
	
	bool EventManager::recievedQuitRequest()
	{
		return EventManager_quitRequest;
	}
	
	void EventManager::update(bool updateInputs)
	{
		if(!Thread::isMainThread())
		{
			return;
		}
		
		Window* resizingWindow = nullptr;
		int data1 = 0;
		int data2 = 0;

		EventManager_quitRequest = false;
		
		Thread::runInAutoreleasePool([&]{
			//event polling
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				bool skip = false;
				if(resizingWindow != nullptr)
				{
					if(event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_RESIZED)
					{
						if(resizingWindow->viewport != nullptr && resizingWindow->viewport->matchesWindow())
						{
							resizingWindow->viewport->setSize((double)event.window.data1, (double)event.window.data2);
						}
						resizingWindow->callListenerEvent(SDL_WINDOWEVENT_RESIZED, event.window.data1, event.window.data2, true);
						skip = true;
					}
					else
					{
						resizingWindow->callListenerEvent(SDL_WINDOWEVENT_RESIZED, data1, data2, false);
					}
					resizingWindow = nullptr;
				}
				
				if(!skip && !EventManager_quitRequest)
				{
					switch(event.type)
					{
						case SDL_MOUSEMOTION:
						{
							Window*window = EventManager::getWindowFromID(event.motion.windowID);
							//TODO add support for multiple mouse indexes
							Mouse::handleMouseMovement(window, 0, Vector2d((double)event.motion.x, (double)event.motion.y), Vector2d((double)event.motion.xrel, (double)event.motion.yrel));
						}
						break;
						
						case SDL_MOUSEBUTTONDOWN:
						case SDL_MOUSEBUTTONUP:
						{
							Window*window = EventManager::getWindowFromID(event.button.windowID);
							Mouse::Button button = Mouse_SDL_to_MouseButton(event.button.button);
							if(event.button.state == SDL_PRESSED)
							{
								//TODO add support for multiple mouse indexes
								Mouse::handleButtonPress(window, 0, button, Vector2d((double)event.button.x, (double)event.button.y));
							}
							else if(event.button.state == SDL_RELEASED)
							{
								//TODO add support for multiple mouse indexes
								Mouse::handleButtonRelease(window, 0, button, Vector2d((double)event.button.x, (double)event.button.y));
							}
						}
						break;
						
						case SDL_FINGERMOTION:
						case SDL_FINGERUP:
						case SDL_FINGERDOWN:
						{
							Window*window = EventManager_windows.get(0);
							Vector2u winSize = window->getSize();
							Vector2d fingerpos = Vector2d(((double)event.tfinger.x)*((double)winSize.x), ((double)event.tfinger.y)*((double)winSize.y));
							if(event.tfinger.type==SDL_FINGERDOWN)
							{
								Multitouch::handleTouchDown(window, (long long)event.tfinger.fingerId, fingerpos);
							}
							else if(event.tfinger.type == SDL_FINGERUP)
							{
								Multitouch::handleTouchUp(window, (long long)event.tfinger.fingerId, fingerpos);
							}
							else if(event.tfinger.type == SDL_FINGERMOTION)
							{
								Vector2d diffingerpos = Vector2d(((double)event.tfinger.dx)*((double)winSize.x), ((double)event.tfinger.dy)*((double)winSize.y));
								Multitouch::handleTouchMove(window, (long long)event.tfinger.fingerId, fingerpos, diffingerpos);
							}
						}
						break;
						
						case SDL_KEYDOWN:
						case SDL_KEYUP:
						{
							if(event.key.state==SDL_PRESSED)
							{
								Keyboard::handleKeyPress(Keyboard_SDLK_to_Key(event.key.keysym.sym));
							}
							else if(event.key.state == SDL_RELEASED)
							{
								Keyboard::handleKeyRelease(Keyboard_SDLK_to_Key(event.key.keysym.sym));
							}
						}
						break;
						
						case SDL_TEXTINPUT:
						{
							Keyboard::handleTextInput(event.text.text);
						}
						break;
						
						case SDL_TEXTEDITING:
						{
							//
						}
						break;
						
						case SDL_WINDOWEVENT:
						{
							Window* window = EventManager::getWindowFromID(event.window.windowID);
							if(window != nullptr)
							{
								if(event.window.event == SDL_WINDOWEVENT_CLOSE)
								{
									bool closeWin = true;
									window->callListenerEvent(event.window.event, event.window.data1, event.window.data2, false, &closeWin);
									if(closeWin)
									{
										window->destroy();
									}
								}
								else if(event.window.event!=SDL_WINDOWEVENT_SIZE_CHANGED)
								{
									window->callListenerEvent(event.window.event, event.window.data1, event.window.data2, false);
								}
								else
								{
									resizingWindow = window;
								}
							}
						}
						break;
						
						case SDL_QUIT:
						EventManager_quitRequest = true;
						break;
						
						//TODO add event types here
					}
				}
				skip = false;
			}
			
			if(resizingWindow != nullptr)
			{
				resizingWindow->callListenerEvent(SDL_WINDOWEVENT_RESIZED, data1, data2, false);
				resizingWindow = nullptr;
			}
			
			if(updateInputs)
			{
				Keyboard::update();
				Mouse::update();
				Multitouch::update();
			}
			Thread::update();
		});
	}
	
	Keyboard::Key Keyboard_SDLK_to_Key(int code)
	{
		switch(code)
		{
			case SDLK_a: return Keyboard::A;
			case SDLK_b: return Keyboard::B;
			case SDLK_c: return Keyboard::C;
			case SDLK_d: return Keyboard::D;
			case SDLK_e: return Keyboard::E;
			case SDLK_f: return Keyboard::F;
			case SDLK_g: return Keyboard::G;
			case SDLK_h: return Keyboard::H;
			case SDLK_i: return Keyboard::I;
			case SDLK_j: return Keyboard::J;
			case SDLK_k: return Keyboard::K;
			case SDLK_l: return Keyboard::L;
			case SDLK_m: return Keyboard::M;
			case SDLK_n: return Keyboard::N;
			case SDLK_o: return Keyboard::O;
			case SDLK_p: return Keyboard::P;
			case SDLK_q: return Keyboard::Q;
			case SDLK_r: return Keyboard::R;
			case SDLK_s: return Keyboard::S;
			case SDLK_t: return Keyboard::T;
			case SDLK_u: return Keyboard::U;
			case SDLK_v: return Keyboard::V;
			case SDLK_w: return Keyboard::W;
			case SDLK_x: return Keyboard::X;
			case SDLK_y: return Keyboard::Y;
			case SDLK_z: return Keyboard::Z;
			case SDLK_0: return Keyboard::NUM_0;
			case SDLK_1: return Keyboard::NUM_1;
			case SDLK_2: return Keyboard::NUM_2;
			case SDLK_3: return Keyboard::NUM_3;
			case SDLK_4: return Keyboard::NUM_4;
			case SDLK_5: return Keyboard::NUM_5;
			case SDLK_6: return Keyboard::NUM_6;
			case SDLK_7: return Keyboard::NUM_7;
			case SDLK_8: return Keyboard::NUM_8;
			case SDLK_9: return Keyboard::NUM_9;
			case SDLK_ESCAPE: return Keyboard::ESCAPE;
			case SDLK_LCTRL: return Keyboard::CTRL_LEFT;
			case SDLK_LSHIFT: return Keyboard::SHIFT_LEFT;
			case SDLK_LALT: return Keyboard::ALT_LEFT;
			case SDLK_LGUI: return Keyboard::WINDOWS_LEFT;
			case SDLK_RCTRL: return Keyboard::CTRL_RIGHT;
			case SDLK_RSHIFT: return Keyboard::SHIFT_RIGHT;
			case SDLK_RALT: return Keyboard::ALT_RIGHT;
			case SDLK_RGUI: return Keyboard::WINDOWS_RIGHT;
			case SDLK_MENU: return Keyboard::CONTEXT_MENU;
			case SDLK_LEFTBRACKET: return Keyboard::OPENBRACKET;
			case SDLK_RIGHTBRACKET: return Keyboard::CLOSEBRACKET;
			case SDLK_SEMICOLON: return Keyboard::SEMICOLON;
			case SDLK_COMMA: return Keyboard::COMMA;
			case SDLK_PERIOD: return Keyboard::PERIOD;
			case SDLK_QUOTE: return Keyboard::QUOTE;
			case SDLK_SLASH: return Keyboard::FORWARDSLASH;
			case SDLK_BACKSLASH: return Keyboard::BACKSLASH;
			case SDLK_EQUALS: return Keyboard::EQUALS;
			case SDLK_MINUS: return Keyboard::DASH;
			case SDLK_SPACE: return Keyboard::SPACE;
			case SDLK_RETURN: return Keyboard::ENTER;
			case SDLK_BACKSPACE: return Keyboard::BACKSPACE;
			case SDLK_TAB: return Keyboard::TAB;
			case SDLK_PAGEUP: return Keyboard::PAGEUP;
			case SDLK_PAGEDOWN: return Keyboard::PAGEDOWN;
			case SDLK_END: return Keyboard::END;
			case SDLK_HOME: return Keyboard::HOME;
			case SDLK_INSERT: return Keyboard::INSERT;
			case SDLK_DELETE: return Keyboard::DELETE;
			case SDLK_PAUSE: return Keyboard::PAUSE;
			case SDLK_KP_PLUS: return Keyboard::ADD;
			case SDLK_KP_MINUS: return Keyboard::SUBTRACT;
			case SDLK_KP_MULTIPLY: return Keyboard::MULTIPLY;
			case SDLK_KP_DIVIDE: return Keyboard::DIVIDE;
			case SDLK_LEFT: return Keyboard::LEFTARROW;
			case SDLK_RIGHT: return Keyboard::RIGHTARROW;
			case SDLK_UP: return Keyboard::UPARROW;
			case SDLK_DOWN: return Keyboard::DOWNARROW;
			case SDLK_KP_0: return Keyboard::NUMPAD_0;
			case SDLK_KP_1: return Keyboard::NUMPAD_1;
			case SDLK_KP_2: return Keyboard::NUMPAD_2;
			case SDLK_KP_3: return Keyboard::NUMPAD_3;
			case SDLK_KP_4: return Keyboard::NUMPAD_4;
			case SDLK_KP_5: return Keyboard::NUMPAD_5;
			case SDLK_KP_6: return Keyboard::NUMPAD_6;
			case SDLK_KP_7: return Keyboard::NUMPAD_7;
			case SDLK_KP_8: return Keyboard::NUMPAD_8;
			case SDLK_KP_9: return Keyboard::NUMPAD_9;
			case SDLK_F1: return Keyboard::F1;
			case SDLK_F2: return Keyboard::F2;
			case SDLK_F3: return Keyboard::F3;
			case SDLK_F4: return Keyboard::F4;
			case SDLK_F5: return Keyboard::F5;
			case SDLK_F6: return Keyboard::F6;
			case SDLK_F7: return Keyboard::F7;
			case SDLK_F8: return Keyboard::F8;
			case SDLK_F9: return Keyboard::F9;
			case SDLK_F10: return Keyboard::F10;
			case SDLK_F11: return Keyboard::F11;
			case SDLK_F12: return Keyboard::F12;
			case SDLK_F13: return Keyboard::F13;
			case SDLK_F14: return Keyboard::F14;
			case SDLK_F15: return Keyboard::F15;
		}
		return Keyboard::UNKNOWN_KEY;
	}
	
	Mouse::Button Mouse_SDL_to_MouseButton(byte button)
	{
		switch(button)
		{
			case SDL_BUTTON_LEFT:
			return Mouse::BUTTON_LEFT;
			
			case SDL_BUTTON_RIGHT:
			return Mouse::BUTTON_RIGHT;
			
			case SDL_BUTTON_MIDDLE:
			return Mouse::BUTTON_MIDDLE;
		}
		return Mouse::UNKNOWN_BUTTON;
	}
}
