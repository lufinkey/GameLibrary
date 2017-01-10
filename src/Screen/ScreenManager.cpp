
#include <GameLibrary/Screen/ScreenManager.hpp>
#include <GameLibrary/Screen/Transition/SlideTransition.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/Screen/ScreenNavigationException.hpp>

namespace fgl
{
	const Transition* const ScreenManager::defaultPushTransition = new SlideTransition(SlideTransition::SLIDE_LEFT);
	
	void ScreenManager::setWindow(Window*win)
	{
		if(window!=win)
		{
			if(window!=nullptr)
			{
				if(childScreen!=nullptr)
				{
					childScreen->setWindow(nullptr);
				}
				for(unsigned int i=0; i<screens.size(); i++)
				{
					screens.get(i)->setWindow(nullptr);
				}
				window = nullptr; //ScreenElement::setWindow(nullptr);
			}
			if(win != nullptr)
			{
				updateFrame(win);
				for(unsigned int i=0; i<screens.size(); i++)
				{
					screens.get(i)->setWindow(win);
				}
				window = win; //ScreenElement::setWindow(win);
				childScreen->setWindow(win);
			}
		}
	}
	
	ScreenManager::ScreenManager(Window*window, Screen*rootScreen) : Screen(window)
	{
		if(rootScreen == nullptr)
		{
			throw IllegalArgumentException("rootScreen", "null");
		}
		else if(rootScreen->screenManager != nullptr)
		{
			throw IllegalArgumentException("rootScreen", "already a part of another ScreenManager");
		}
		else if(rootScreen->parentScreen != nullptr)
		{
			throw IllegalArgumentException("rootScreen", "already presented on another Screen");
		}
		else if(rootScreen->window != nullptr)
		{
			throw IllegalArgumentException("rootScreen", "already belongs to another Window");
		}
		TransitionData_clear(pushpopData);
		rootScreen->setWindow(window);
		rootScreen->screenManager = this;
		screens.add(rootScreen);
	}
	
	ScreenManager::ScreenManager(Screen*rootScreen) : ScreenManager(nullptr, rootScreen)
	{
		//
	}
	
	ScreenManager::~ScreenManager()
	{
		for(unsigned int i=0; i<screens.size(); i++)
		{
			Screen* screen = screens.get(i);
			screen->screenManager = nullptr;
			//screen->parentElement = nullptr;
		}
		screens.clear();
	}
	
	void ScreenManager::onWillAppear(const Transition*transition)
	{
		Screen::onWillAppear(transition);
		if(pushpopData.action==TRANSITION_NONE)
		{
			screens.get(screens.size()-1)->onWillAppear(transition);
		}
	}
	
	void ScreenManager::onDidAppear(const Transition*transition)
	{
		Screen::onDidAppear(transition);
		if(pushpopData.action==TRANSITION_NONE)
		{
			screens.get(screens.size()-1)->onDidAppear(transition);
		}
	}
	
	void ScreenManager::onWillDisappear(const Transition*transition)
	{
		Screen::onWillDisappear(transition);
		screens.get(screens.size()-1)->onWillDisappear(transition);
	}
	
	void ScreenManager::onDidDisappear(const Transition*transition)
	{
		Screen::onDidDisappear(transition);
		screens.get(screens.size()-1)->onDidDisappear(transition);
	}
	
	void ScreenManager::update(ApplicationData appData)
	{
		if(!isshown)
		{
			isshown = true;
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}
		
		Screen* updateCaller = nullptr;
		Screen* pushpop_onDidDisappearCaller = nullptr;
		Screen* pushpop_onDidAppearCaller = nullptr;

		TransitionData_checkFinished(appData, pushpopData, &pushpop_onDidDisappearCaller, &pushpop_onDidAppearCaller);

		if(childScreen==nullptr)
		{
			if(pushpopData.action==TRANSITION_NONE)
			{
				updateCaller = screens.get(screens.size()-1);
			}
			TransitionData_callVirtualFunctions(pushpopData, pushpop_onDidDisappearCaller, pushpop_onDidAppearCaller);
		}

		if(updateCaller != nullptr)
		{
			updateCaller->update(appData);
		}
		
		Screen::update(appData);

		TransitionData_checkInitialization(appData, pushpopData);
	}
	
	void ScreenManager::drawScreens(ApplicationData appData, Graphics graphics) const
	{
		if(childScreen==nullptr || overlayData.action!=TRANSITION_NONE)
		{
			RectangleD frame = getFrame();
			graphics.translate(frame.x, frame.y);

			if(pushpopData.action == TRANSITION_NONE)
			{
				screens.get(screens.size()-1)->draw(appData, graphics);
			}
			else
			{
				double progress = pushpopData.progress;

				if(pushpopData.action == TRANSITION_HIDE)
				{
					progress = 1 - progress;
				}

				pushpopData.transition->draw(appData, graphics, progress, static_cast<Drawable*>(pushpopData.screen), static_cast<Drawable*>(pushpopData.transitionScreen));
			}
		}
	}
	
	void ScreenManager::draw(ApplicationData appData, Graphics graphics) const
	{
		if(overlayData.action==TRANSITION_NONE)
		{
			drawScreens(appData, graphics);
			drawElements(appData, graphics);
			drawOverlay(appData, graphics);
		}
		else if(drawingOverlayTransition)
		{
			drawBackground(appData, graphics);
			drawScreens(appData, graphics);
			drawElements(appData, graphics);
		}
		else
		{
			drawingOverlayTransition = true;
			drawOverlay(appData, graphics);
			drawingOverlayTransition = false;
		}
	}
	
	void ScreenManager::set(const ArrayList<Screen*>& newScreens, const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(!isshown)
		{
			isshown = true;
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}
		
		if(pushpopData.action != TRANSITION_NONE)
		{
			throw ScreenNavigationException("Cannot set a ScreenManager's screens while the ScreenManager is performing a transition");
		}
		else if(newScreens.size()==0)
		{
			throw IllegalArgumentException("screens", "empty list");
		}
		else
		{
			for(unsigned int i=0; i<newScreens.size(); i++)
			{
				Screen* screen = newScreens.get(i);
				if(screen == nullptr)
				{
					throw IllegalArgumentException("screens", "list contains null");
				}
				else if(screen->screenManager != nullptr && screen->screenManager!=this)
				{
					throw IllegalArgumentException("screens", "list contains Screen that is already part of another ScreenManager");
				}
				else if(screen->parentScreen != nullptr)
				{
					throw IllegalArgumentException("screens", "list contains Screen that is already presented on another Screen");
				}
				/*else if(screen->parentElement != nullptr && (screen->parentElement!=this || screen->screenManager!=this))
				{
					throw IllegalArgumentException("screens", "list contains Screen that is already added to a ScreenElement");
				}*/
				else if(screen->window != nullptr && screen->screenManager!=this)
				{
					throw IllegalArgumentException("screens", "list contains Screen that belongs to another Window");
				}
				else
				{
					for(unsigned int j=(i+1); j<newScreens.size(); j++)
					{
						Screen* screen2 = newScreens.get(j);
						if(screen == screen2)
						{
							throw IllegalArgumentException("screens", "list contains duplicates");
						}
					}
				}
			}

			Screen* lastNewScreen = newScreens.get(newScreens.size()-1);
			Screen* currentScreen = screens.get(screens.size() - 1);
			if(lastNewScreen == currentScreen)
			{
				for(unsigned int i=0; i<screens.size(); i++)
				{
					Screen* screen = screens.get(i);
					screen->screenManager = nullptr;
					//screen->parentElement = nullptr;
					screen->setWindow(nullptr);
				}
				screens = newScreens;
				for(unsigned int i=0; i<screens.size(); i++)
				{
					Screen* screen = screens.get(i);
					screen->screenManager = this;
					//screen->parentElement = this;
					screen->setWindow(window);
				}

				if(completion != nullptr)
				{
					completion((void*)this);
				}
			}
			else
			{
				Screen* topOldScreen = currentScreen->getTopScreen();
				Screen* topNewScreen = lastNewScreen->getTopScreen();
				bool ontop = isOnTop();
				if(screens.contains(lastNewScreen))
				{
					//pop animation
					TransitionData_begin(pushpopData, lastNewScreen, currentScreen, TRANSITION_HIDE, transition, duration, completion, (void*)this);
				}
				else
				{
					//push animation
					TransitionData_begin(pushpopData, currentScreen, lastNewScreen, TRANSITION_SHOW, transition, duration, completion, (void*)this);
				}
				for(unsigned int i=0; i<screens.size(); i++)
				{
					Screen*screen = screens.get(i);
					screen->screenManager = nullptr;
					//screen->parentElement = nullptr;
					screen->setWindow(nullptr);
				}
				screens = newScreens;
				for(unsigned int i=0; i<screens.size(); i++)
				{
					Screen*screen = screens.get(i);
					screen->screenManager = this;
					//screen->parentElement = this;
					screen->setWindow(window);
				}

				if(transition == nullptr || duration == 0)
				{
					if(ontop)
					{
						topOldScreen->onWillDisappear(transition);
						topNewScreen->onWillAppear(transition);

						TransitionData_clear(pushpopData);

						topOldScreen->onDidDisappear(transition);
						topNewScreen->onDidAppear(transition);

						if(completion != nullptr)
						{
							completion((void*)this);
						}
					}
					else
					{
						TransitionData_clear(overlayData);

						if(completion != nullptr)
						{
							completion((void*)this);
						}
					}
				}
				else
				{
					if(ontop)
					{
						topOldScreen->onWillDisappear(transition);
						topNewScreen->onWillAppear(transition);
					}
				}
			}
		}
	}
	
	void ScreenManager::push(Screen*screen, const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(!isshown)
		{
			isshown = true;
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}
		
		ArrayList<Screen*> screens(1);
		screens.set(0, screen);
		push(screens, transition, duration, completion);
	}
	
	void ScreenManager::push(const ArrayList<Screen*>& newScreens, const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(!isshown)
		{
			isshown = true;
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}

		if(pushpopData.action != TRANSITION_NONE)
		{
			throw ScreenNavigationException("Cannot push Screens on a ScreenManager while the ScreenManager is performing a transition");
		}
		else if(newScreens.size()==0)
		{
			throw IllegalArgumentException("screens", "empty list");
		}
		else
		{
			for(unsigned int i=0; i<newScreens.size(); i++)
			{
				Screen* screen = newScreens.get(i);
				if(screen == nullptr)
				{
					throw IllegalArgumentException("screens", "list contains null");
				}
				else if(screen->screenManager != nullptr)
				{
					throw IllegalArgumentException("screens", "list contains Screen that is already a part of a ScreenManager");
				}
				else if(screen->parentScreen != nullptr)
				{
					throw IllegalArgumentException("screen", "list contains Screen that is already presented on another Screen");
				}
				/*else if(screen->parentElement != nullptr)
				{
					throw IllegalArgumentException("screen", "list contains Screen that is already added to a ScreenElement");
				}*/
				else if(screen->window != nullptr)
				{
					throw IllegalArgumentException("screen", "list contains Screen that already belongs to another Window");
				}
				else
				{
					for(unsigned int j=(i+1); j<newScreens.size(); j++)
					{
						Screen* screen2 = newScreens.get(j);
						if(screen == screen2)
						{
							throw IllegalArgumentException("screens", "list contains duplicates");
						}
					}
				}
			}
			
			Screen* lastNewScreen = newScreens.get(newScreens.size()-1);
			Screen* currentScreen = screens.get(screens.size() - 1);
			
			Screen* topOldScreen = currentScreen->getTopScreen();
			Screen* topNewScreen = lastNewScreen->getTopScreen();
			
			bool ontop = isOnTop();
			TransitionData_begin(pushpopData, currentScreen, lastNewScreen, TRANSITION_SHOW, transition, duration, completion, (void*)this);
			for(unsigned int i=0; i<newScreens.size(); i++)
			{
				Screen* screen = newScreens.get(i);
				screen->screenManager = this;
				//screen->parentElement = this;
				screen->setWindow(window);
				screens.add(screen);
			}
			if(transition == nullptr || duration == 0)
			{
				if(ontop)
				{
					topOldScreen->onWillDisappear(transition);
					topNewScreen->onWillAppear(transition);

					TransitionData_clear(pushpopData);

					topOldScreen->onDidDisappear(transition);
					topNewScreen->onDidAppear(transition);

					if(completion != nullptr)
					{
						completion((void*)this);
					}
				}
				else
				{
					TransitionData_clear(overlayData);

					if(completion != nullptr)
					{
						completion((void*)this);
					}
				}
			}
			else
			{
				if(ontop)
				{
					topOldScreen->onWillDisappear(transition);
					topNewScreen->onWillAppear(transition);
				}
			}
		}
	}
	
	Screen* ScreenManager::pop(const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(pushpopData.action != TRANSITION_NONE)
		{
			throw ScreenNavigationException("Cannot pop a Screen on a ScreenManager while the ScreenManager is performing a transition");
		}
		else if(screens.size() == 1)
		{
			throw ScreenNavigationException("Cannot pop any more Screens from ScreenManager; A ScreenManager must have atleast 1 Screen");
		}
		else
		{
			return popTo(screens.get(screens.size()-2), transition, duration, completion).get(0);
		}
	}
	
	ArrayList<Screen*> ScreenManager::popTo(Screen*screen, const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(pushpopData.action != TRANSITION_NONE)
		{
			throw ScreenNavigationException("Cannot pop a Screen on a ScreenManager while the ScreenManager is performing a transition");
		}
		else if(screen == screens.get(screens.size() - 1))
		{
			return ArrayList<Screen*>();
		}
		else
		{
			size_t index = screens.indexOf(screen);
			if(index == ArrayList<Screen*>::NOT_FOUND)
			{
				throw IllegalArgumentException("screen", "not contained within the calling ScreenManager");
			}

			Screen* newScreen = screen;
			Screen* currentScreen = screens.get(screens.size()-1);

			ArrayList<Screen*> popped;
			for(size_t i=(index+1); i<screens.size(); i++)
			{
				Screen* screen = screens.get(i);
				//screen->parentElement = nullptr;
				screen->screenManager = nullptr;
				screen->setWindow(nullptr);
				popped.add(screen);
			}
			while(screens.size()>(index + 1))
			{
				screens.remove(screens.size()-1);
			}

			Screen* topOldScreen = currentScreen->getTopScreen();
			Screen* topNewScreen = newScreen->getTopScreen();

			bool ontop = isOnTop();
			TransitionData_begin(pushpopData, newScreen, currentScreen, TRANSITION_HIDE, transition, duration, completion, (void*)this);

			if(transition == nullptr || duration == 0)
			{
				if(ontop)
				{
					topOldScreen->onWillDisappear(transition);
					topNewScreen->onWillAppear(transition);

					TransitionData_clear(pushpopData);

					topOldScreen->onDidDisappear(transition);
					topNewScreen->onDidAppear(transition);

					if(completion != nullptr)
					{
						completion((void*)this);
					}
				}
				else
				{
					TransitionData_clear(overlayData);

					if(completion != nullptr)
					{
						completion((void*)this);
					}
				}
			}
			else
			{
				if(ontop)
				{
					topOldScreen->onWillDisappear(transition);
					topNewScreen->onWillAppear(transition);
				}
			}

			return popped;
		}
	}
	
	ArrayList<Screen*> ScreenManager::popToRoot(const Transition*transition, unsigned long long duration, CompletionCallback completion)
	{
		if(pushpopData.action != TRANSITION_NONE)
		{
			throw ScreenNavigationException("Cannot pop a Screen on a ScreenManager while the ScreenManager is performing a transition");
		}
		else
		{
			return popTo(screens.get(0), transition, duration, completion);
		}
	}
}
