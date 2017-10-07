
#include <GameLibrary/Screen/ScreenManager.hpp>
#include <GameLibrary/Screen/Transition/SlideTransition.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/Screen/ScreenNavigationException.hpp>

namespace fgl
{
	const Transition* const ScreenManager::defaultPushTransition = new SlideTransition(SlideTransition::SLIDE_LEFT);

	ScreenManager::ScreenManager(Window* window, Screen* rootScreen) : Screen(window)
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
		rootScreen->screenManager = this;
		screens.add(rootScreen);
		addChildScreen(rootScreen, 0.4);
	}
	
	ScreenManager::ScreenManager(Screen* rootScreen) : ScreenManager(nullptr, rootScreen)
	{
		//
	}
	
	ScreenManager::~ScreenManager()
	{
		//
	}
	
	void ScreenManager::setScreens(const ArrayList<Screen*>& newScreens, const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		handleFirstShowing();
		
		if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be a negative value");
		}
		else if(isTransitionAnimating("pushpop"))
		{
			throw ScreenNavigationException("Cannot set a ScreenManager's screens while the ScreenManager is performing a transition");
		}
		else if(newScreens.size()==0)
		{
			throw IllegalArgumentException("screens", "cannot be an empty list");
		}
		else
		{
			for(size_t i=0; i<newScreens.size(); i++)
			{
				auto& screen = newScreens.get(i);
				if(screen == nullptr)
				{
					throw IllegalArgumentException("screens", (String)"contains null Screen at index "+i);
				}
				else if(screen->screenManager != nullptr && screen->screenManager!=this)
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that is already part of another ScreenManager at index "+i);
				}
				else if(screen->parentScreen != nullptr && (screen->parentScreen!=this || (screen->parentScreen==this && screen->screenManager!=this)))
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that is already a child of another Screen at index "+i);
				}
				else if(screen->window != nullptr && screen->screenManager!=this)
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that belongs to another Window at index "+i);
				}
				else
				{
					for(size_t j=(i+1); j<newScreens.size(); j++)
					{
						Screen* screen2 = newScreens.get(j);
						if(screen == screen2)
						{
							throw IllegalArgumentException("screens", (String)"contains duplicate Screens at index "+i+" and index "+j);
						}
					}
				}
			}

			Screen* lastNewScreen = newScreens.get(newScreens.size()-1);
			Screen* currentScreen = screens.get(screens.size() - 1);

			Screen* topOldScreen = currentScreen->getTopScreenInHeirarchy();
			Screen* topNewScreen = lastNewScreen->getTopScreenInHeirarchy();
			bool visible = isOnTop();

			//set new screens
			for(auto& screen : screens)
			{
				screen->screenManager = nullptr;
				removeChildScreen(screen);
			}
			screens = newScreens;
			for(auto& screen : screens)
			{
				screen->screenManager = this;
				addChildScreen(screen, 0.4, false);
			}
			setChildScreenVisible(screens[screens.size()-1], true);

			if(lastNewScreen == currentScreen)
			{
				//no transition needed if the top screens are the same
				if(oncompletion)
				{
					oncompletion();
				}
			}
			else
			{
				//transition between the screens

				if(visible)
				{
					topOldScreen->onWillDisappear(transition);
					topNewScreen->onWillAppear(transition);
				}

				if(screens.contains(lastNewScreen))
				{
					//pop animation
					addTransition("pushpop", 0.4, lastNewScreen, currentScreen, TRANSITION_HIDE, transition, duration, [=]{
						if(visible)
						{
							topOldScreen->onDidDisappear(transition);
							topNewScreen->onDidAppear(transition);
						}
						if(oncompletion)
						{
							oncompletion();
						}
					});
				}
				else
				{
					//push animation
					addTransition("pushpop", 0.4, currentScreen, lastNewScreen, TRANSITION_SHOW, transition, duration, [=]{
						if(visible)
						{
							topOldScreen->onDidDisappear(transition);
							topNewScreen->onDidAppear(transition);
						}
						if(oncompletion)
						{
							oncompletion();
						}
					});
				}
			}
		}
	}
	
	void ScreenManager::pushScreen(Screen* screen, const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		handleFirstShowing();

		pushScreens({screen}, transition, duration, oncompletion);
	}
	
	void ScreenManager::pushScreens(const ArrayList<Screen*>& newScreens, const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		handleFirstShowing();

		if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be a negative value");
		}
		else if(isTransitionAnimating("pushpop"))
		{
			throw ScreenNavigationException("Cannot push Screens on a ScreenManager while the ScreenManager is performing a transition");
		}
		else if(newScreens.size()==0)
		{
			throw IllegalArgumentException("screens", "cannot be an empty list");
		}
		else
		{
			for(size_t i=0; i<newScreens.size(); i++)
			{
				Screen* screen = newScreens.get(i);
				if(screen == nullptr)
				{
					throw IllegalArgumentException("screens", (String)"contains null Screen at index "+i);
				}
				else if(screen->screenManager != nullptr)
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that is already part of another ScreenManager at index "+i);
				}
				else if(screen->parentScreen != nullptr)
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that is already a child of another Screen at index "+i);
				}
				else if(screen->window != nullptr)
				{
					throw IllegalArgumentException("screens", (String)"contains Screen that belongs to another Window at index "+i);
				}
				else
				{
					for(size_t j=(i+1); j<newScreens.size(); j++)
					{
						Screen* screen2 = newScreens.get(j);
						if(screen == screen2)
						{
							throw IllegalArgumentException("screens", (String)"contains duplicate Screens at index "+i+" and index "+j);
						}
					}
				}
			}
			
			Screen* lastNewScreen = newScreens.get(newScreens.size()-1);
			Screen* currentScreen = screens.get(screens.size() - 1);
			
			Screen* topOldScreen = currentScreen->getTopScreenInHeirarchy();
			Screen* topNewScreen = lastNewScreen->getTopScreenInHeirarchy();
			bool visible = isOnTop();

			setChildScreenVisible(screens[screens.size()-1], false);
			screens.addAll(newScreens);
			for(auto& screen : newScreens)
			{
				screen->screenManager = this;
				addChildScreen(screen, 0.4, false);
			}
			setChildScreenVisible(screens[screens.size()-1], true);
			
			if(visible)
			{
				topOldScreen->onWillDisappear(transition);
				topNewScreen->onWillAppear(transition);
			}

			addTransition("pushpop", 0.4, currentScreen, lastNewScreen, TRANSITION_SHOW, transition, duration, [=]{
				if(visible)
				{
					topOldScreen->onDidDisappear(transition);
					topNewScreen->onDidAppear(transition);
				}

				if(oncompletion)
				{
					oncompletion();
				}
			});
		}
	}
	
	Screen* ScreenManager::popScreen(const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		if(isTransitionAnimating("pushpop"))
		{
			throw ScreenNavigationException("Cannot pop a Screen on a ScreenManager while the ScreenManager is performing a transition");
		}
		else if(screens.size() == 1)
		{
			throw ScreenNavigationException("Cannot pop any more Screens from ScreenManager; A ScreenManager must have atleast 1 Screen");
		}
		return popToScreen(screens.get(screens.size()-2), transition, duration, oncompletion).get(0);
	}
	
	ArrayList<Screen*> ScreenManager::popToScreen(Screen* screen, const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be a negative value");
		}
		else if(isTransitionAnimating("pushpop"))
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
				Screen* poppedScreen = screens[i];
				poppedScreen->screenManager = nullptr;
				removeChildScreen(poppedScreen);
				popped.add(poppedScreen);
			}
			while(screens.size() > (index + 1))
			{
				screens.remove(screens.size()-1);
			}
			setChildScreenVisible(screens[screens.size()-1], true);

			Screen* topOldScreen = currentScreen->getTopScreenInHeirarchy();
			Screen* topNewScreen = newScreen->getTopScreenInHeirarchy();
			bool visible = isOnTop();

			if(visible)
			{
				topOldScreen->onWillDisappear(transition);
				topNewScreen->onWillAppear(transition);
			}

			addTransition("pushpop", 0.4, newScreen, currentScreen, TRANSITION_HIDE, transition, duration, [=]{
				if(visible)
				{
					topOldScreen->onDidDisappear(transition);
					topNewScreen->onDidAppear(transition);
				}
				if(oncompletion)
				{
					oncompletion();
				}
			});

			return popped;
		}
	}
	
	ArrayList<Screen*> ScreenManager::popToFirstScreen(const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		if(isTransitionAnimating("pushpop"))
		{
			throw ScreenNavigationException("Cannot pop a Screen on a ScreenManager while the ScreenManager is performing a transition");
		}
		else
		{
			return popToScreen(screens.get(0), transition, duration, oncompletion);
		}
	}

	const ArrayList<Screen*>& ScreenManager::getScreens() const
	{
		return screens;
	}
}
