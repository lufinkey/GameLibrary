
#include <GameLibrary/Screen/Screen.hpp>
#include <GameLibrary/Screen/ScreenManager.hpp>
#include <GameLibrary/Screen/Transition/PopoverTransition.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>
#include <GameLibrary/Exception/Screen/ScreenNavigationException.hpp>

namespace fgl
{
	const Transition* const Screen::defaultPresentationTransition = new PopoverTransition(PopoverTransition::POPOVER_UP);
	
	void Screen::updateFrame(Window* window)
	{
		if(window != nullptr)
		{
			Viewport* viewport = window->getViewport();
			Vector2d size;
			if(viewport != nullptr)
			{
				if(viewport->matchesWindow())
				{
					size = (Vector2d)window->getSize();
				}
				else
				{
					size = viewport->getSize();
				}
			}
			else
			{
				size = (Vector2d)window->getSize();
			}
			if(framesize.x!=size.x || framesize.y!=size.y)
			{
				Vector2d oldSize = framesize;
				framesize = size; //setFrame(RectangleD(frame.x, frame.y, size.x, size.y));
				ScreenElement* mainElement = getElement();
				mainElement->setFrame(RectangleD(0,0,size.x,size.y));
				onSizeChange(oldSize, size);
			}
		}
	}
	
	void Screen::setWindow(Window* win)
	{
		if(window!=win)
		{
			if(window!=nullptr)
			{
				if(childScreen!=nullptr)
				{
					childScreen->setWindow(nullptr);
				}
				window = nullptr; //ScreenElement::setWindow(nullptr);
			}
			if(win != nullptr)
			{
				updateFrame(win);
				window = win; //ScreenElement::setWindow(win);
				if(childScreen!=nullptr)
				{
					childScreen->setWindow(win);
				}
			}
			updateFrame(win);
		}
	}
	
	void Screen::TransitionData_clear(TransitionData& data)
	{
		data.screen = nullptr;
		data.transitionScreen = nullptr;
		data.action = TRANSITION_NONE;
		data.requiresInitializing = false;
		data.startTime = 0;
		data.transition = nullptr;
		data.progress = 0;
		data.duration = 0;
		data.completion = nullptr;
	}
	
	void Screen::TransitionData_begin(TransitionData& data, Screen* screen, Screen* transitionScreen, TransitionAction action, const Transition* transition, long long duration, const std::function<void()>& completion)
	{
		data.screen = screen;
		data.transitionScreen = transitionScreen;
		data.action = action;
		data.requiresInitializing = true;
		data.startTime = 0;
		data.transition = transition;
		data.progress = 0;
		data.duration = duration;
		data.completion = completion;
	}
	
	void Screen::TransitionData_checkInitialization(ApplicationData& appData, Screen::TransitionData&data)
	{
		if(data.requiresInitializing)
		{
			data.startTime = appData.getTime().getMilliseconds();
			data.requiresInitializing = false;
		}
	}
	
	byte Screen::TransitionData_applyProgress(ApplicationData& appData, Screen::TransitionData&data)
	{
		byte finishedAction = TRANSITION_NONE;
		if(data.action != TRANSITION_NONE)
		{
			TransitionData_checkInitialization(appData, data);
			long long currentTime = appData.getTime().getMilliseconds();
			long long timeDif = currentTime - data.startTime;
			data.progress = (double)(((double)timeDif) / ((double)data.duration));
			if(data.progress >= 1)
			{
				finishedAction = data.action;
			}
		}
		return finishedAction;
	}
	
	std::function<void()> Screen::TransitionData_checkFinished(ApplicationData& appData, Screen::TransitionData& data)
	{
		//apply any progress to the presenting transition
		const Transition* transition = data.transition;
		std::function<void()> completion = data.completion;
		byte finishedAction = TransitionData_applyProgress(appData, data);
		Screen* onDidDisappearCaller = nullptr;
		Screen* onDidAppearCaller = nullptr;
		if(finishedAction == TRANSITION_HIDE)
		{
			onDidDisappearCaller = data.transitionScreen;
			onDidAppearCaller = data.screen;
			TransitionData_clear(data);
		}
		else if(finishedAction == TRANSITION_SHOW)
		{
			onDidDisappearCaller = data.screen;
			onDidAppearCaller = data.transitionScreen;
			TransitionData_clear(data);
		}

		if(finishedAction==TRANSITION_HIDE || finishedAction==TRANSITION_SHOW)
		{
			return [=]{
				if(onDidDisappearCaller!=nullptr && !onDidDisappearCaller->isOnTop())
				{
					onDidDisappearCaller->getTopScreen()->onDidDisappear(transition);
				}
				if(onDidAppearCaller!=nullptr && onDidAppearCaller->isOnTop())
				{
					onDidAppearCaller->getTopScreen()->onDidAppear(transition);
				}
				if(completion)
				{
					completion();
				}
			};
		}
		return nullptr;
	}

	void Screen::handleFirstShowing()
	{
		if(!isshown)
		{
			isshown = true;
			element->layoutChildElements();
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}
	}
	
	Screen::Screen(Window* window_arg)
	{
		element = new ScreenElement(RectangleD(0,0, framesize.x, framesize.y));
		screenManager = nullptr;
		parentScreen = nullptr;
		childScreen = nullptr;
		drawingOverlayTransition = false;
		isshown = false;
		drawsParent = false;
		updatesParent = false;

		TransitionData_clear(overlayData);

		window = window_arg;
		if(window != nullptr)
		{
			Viewport*view = window->getViewport();
			if(view != nullptr)
			{
				const Vector2d& size = view->getSize();
				if(framesize.x!=size.x || framesize.y!=size.y)
				{
					framesize = size; //frame = RectangleD(frame.x, frame.y, size.x, size.y);
					element->setFrame(RectangleD(0,0,size.x,size.y));
				}
			}
		}
	}
	
	Screen::Screen() : Screen(nullptr)
	{
		//
	}
	
	Screen::~Screen()
	{
		if(childScreen!=nullptr)
		{
			//childScreen->parentElement = nullptr;
			childScreen->parentScreen = nullptr;
			childScreen = nullptr;
		}
		delete element;
	}
	
	void Screen::onSizeChange(const Vector2d& oldFrame, const Vector2d& newFrame)
	{
		//Open for implementation
	}
	
	void Screen::onWillAppear(const Transition* transition)
	{
		//Open for implementation
	}
	
	void Screen::onDidAppear(const Transition* transition)
	{
		//Open for implementation
	}
	
	void Screen::onWillDisappear(const Transition* transition)
	{
		//Open for implementation
	}
	
	void Screen::onDidDisappear(const Transition* transition)
	{
		//Open for implementation
	}
	
	void Screen::update(ApplicationData appData)
	{
		handleFirstShowing();

		Screen* updateCaller = nullptr;
		
		auto transitionFinishHandler = TransitionData_checkFinished(appData, overlayData);
		
		if(childScreen!=nullptr)
		{
			updateCaller = childScreen;
		}
		
		updateFrame(window);
		
		if(transitionFinishHandler)
		{
			transitionFinishHandler();
		}
		
		updateFrame(window);

		if(updateCaller==nullptr || updateCaller->updatesParent)
		{
			if(element == nullptr)
			{
				getElement();
			}
			onUpdate(appData);
			if(Multitouch::isAvailable())
			{
				updateElementTouch(appData);
			}
			else
			{
				updateElementMouse(appData);
			}
			element->update(appData);
		}
		
		if(updateCaller != nullptr)
		{
			updateCaller->update(appData);
		}
		
		TransitionData_checkInitialization(appData, overlayData);
		
		updateFrame(window);
	}
	
	void Screen::onUpdate(const ApplicationData& appData)
	{
		//Open for implementation
	}
	
	void Screen::drawElements(ApplicationData appData, Graphics graphics) const
	{
		if(childScreen==nullptr || childScreen->drawsParent || overlayData.action != TRANSITION_NONE)
		{
			if(element != nullptr)
			{
				element->draw(appData, graphics);
			}
			//ScreenElement::drawElements(appData, graphics);
			onDraw(appData, graphics);
		}
	}
	
	void Screen::drawOverlay(ApplicationData appData, Graphics graphics) const
	{
		if(childScreen!=nullptr || overlayData.action!=TRANSITION_NONE)
		{
			if(overlayData.action == TRANSITION_NONE)
			{
				Vector2d overlaySize = childScreen->getSize();
				double xOff = (overlaySize.x - framesize.x)/2;
				double yOff = (overlaySize.y - framesize.y)/2;
				graphics.translate(xOff, yOff);
				childScreen->draw(appData, graphics);
			}
			else
			{
				double progress = overlayData.progress;
				
				if(overlayData.action == TRANSITION_HIDE)
				{
					progress = 1 - progress;
				}
				
				overlayData.transition->draw(appData, graphics, progress, overlayData.screen, overlayData.transitionScreen);
			}
		}
	}
	
	void Screen::draw(ApplicationData appData, Graphics graphics) const
	{
		if(overlayData.action==TRANSITION_NONE)
		{
			drawElements(appData, graphics);
			drawOverlay(appData, graphics);
		}
		else if(drawingOverlayTransition)
		{
			drawElements(appData, graphics);
		}
		else
		{
			drawingOverlayTransition = true;
			drawOverlay(appData, graphics);
			drawingOverlayTransition = false;
		}
	}
	
	void Screen::onDraw(const ApplicationData& appData, Graphics graphics) const
	{
		//Open for implementation
	}
	
	const Vector2d& Screen::getSize() const
	{
		return framesize;
	}
	
	const Transition* Screen::getDefaultPresentationTransition() const
	{
		return defaultPresentationTransition;
	}
	
	long long Screen::getDefaultPresentationDuration() const
	{
		return Transition::defaultDuration;
	}
	
	const Transition* Screen::getDefaultDismissalTransition() const
	{
		return defaultPresentationTransition;
	}
	
	long long Screen::getDefaultDismissalDuration() const
	{
		return Transition::defaultDuration;
	}
	
	void Screen::presentChildScreen(Screen* screen, const Transition* transition, long long duration, const std::function<void()>& oncompletion, const std::function<void()>& ondismissal)
	{
		handleFirstShowing();
		
		if(screen == nullptr)
		{
			throw IllegalArgumentException("screen", "cannot be null");
		}
		else if(screen->parentScreen != nullptr)
		{
			throw IllegalArgumentException("screen", "already presenting on another Screen");
		}
		/*else if(screen->parentElement != nullptr)
		{
			throw IllegalArgumentException("screen", "already displaying on another ScreenElement");
		}*/
		else if(screen->screenManager != nullptr)
		{
			throw IllegalArgumentException("screen", "already displaying in a ScreenManager");
		}
		else if(screen->window != nullptr)
		{
			throw IllegalArgumentException("screen", "already belongs to a Window");
		}
		else if(childScreen != nullptr)
		{
			//if(overlayData.action != TRANSITION_NONE)
			//{
				throw ScreenNavigationException("Cannot present Screen on top of a screen that is currently presenting or dismissing");
			//}
			//childScreen->present(screen, transition, duration);
		}
		else if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be negative");
		}
		else
		{
			Screen* topScreen = screen->getTopScreen();
			bool visible = isOnTop();
			TransitionData_begin(overlayData, this, screen, TRANSITION_SHOW, transition, duration, oncompletion);
			childScreen = screen;
			childScreenDismissCallback = ondismissal;
			childScreen->setWindow(window);
			childScreen->parentScreen = this;
			//childScreen->parentElement = this;
			if(transition == nullptr || duration==0)
			{
				if(visible)
				{
					onWillDisappear(transition);
					topScreen->onWillAppear(transition);
					
					TransitionData_clear(overlayData);
					
					onDidDisappear(transition);
					topScreen->onDidAppear(transition);
					
					if(oncompletion)
					{
						oncompletion();
					}
				}
				else
				{
					TransitionData_clear(overlayData);
					
					if(oncompletion)
					{
						oncompletion();
					}
				}
			}
			else
			{
				if(visible)
				{
					onWillDisappear(transition);
					topScreen->onWillAppear(transition);
				}
			}
		}
	}
	
	void Screen::presentChildScreen(Screen* screen, const std::function<void()>& oncompletion, const std::function<void()>& ondismissal)
	{
		if(screen == nullptr)
		{
			throw IllegalArgumentException("screen", "cannot be null");
		}
		presentChildScreen(screen, screen->getDefaultPresentationTransition(), screen->getDefaultDismissalDuration(), oncompletion, ondismissal);
	}
	
	void Screen::dismissChildScreen(const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		if(childScreen == nullptr)
		{
			throw ScreenNavigationException("No Screen is being presented that can be dismissed");
		}
		else if(overlayData.action==TRANSITION_HIDE)
		{
			throw ScreenNavigationException("Cannot dismiss a Screen that is already dismissing");
		}
		else if(overlayData.action==TRANSITION_SHOW)
		{
			throw ScreenNavigationException("Cannot dismiss a Screen that is in the transition of presenting");
		}
		else if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be negative");
		}
		else
		{
			std::function<void()> completionHandler([=]{
				auto dismissHandler = childScreenDismissCallback;
				childScreenDismissCallback = nullptr;
				if(oncompletion)
				{
					oncompletion();
				}
				if(dismissHandler)
				{
					dismissHandler();
				}
			});

			Screen* topScreen = childScreen->getTopScreen();
			bool visible = topScreen->isOnTop();
			TransitionData_begin(overlayData, this, childScreen, TRANSITION_HIDE, transition, duration, completionHandler);
			childScreen->setWindow(nullptr);
			childScreen->parentScreen = nullptr;
			//childScreen->parentElement = nullptr;
			childScreen = nullptr;
			
			if(transition == nullptr || duration==0)
			{
				if(visible)
				{
					topScreen->onWillDisappear(transition);
					onWillAppear(transition);
					
					TransitionData_clear(overlayData);
					
					topScreen->onDidDisappear(transition);
					onDidAppear(transition);
					
					if(completionHandler)
					{
						completionHandler();
					}
				}
				else
				{
					TransitionData_clear(overlayData);
					
					if(completionHandler)
					{
						completionHandler();
					}
				}
			}
			else
			{
				if(visible)
				{
					topScreen->onWillDisappear(transition);
					onWillAppear(transition);
				}
			}
		}
	}
	
	void Screen::dismissChildScreen(const std::function<void()>& completion)
	{
		if(childScreen == nullptr)
		{
			throw ScreenNavigationException("No Screen is being presented that can be dismissed");
		}
		dismissChildScreen(childScreen->getDefaultDismissalTransition(), childScreen->getDefaultDismissalDuration(), completion);
	}
	
	ScreenElement* Screen::getElement() const
	{
		return element;
	}
	
	ScreenManager* Screen::getScreenManager() const
	{
		return screenManager;
	}
	
	Screen* Screen::getParentScreen() const
	{
		return parentScreen;
	}
	
	Screen* Screen::getChildScreen() const
	{
		return childScreen;
	}
	
	Screen* Screen::getTopScreen()
	{
		if(childScreen == nullptr)
		{
			return this;
		}
		return childScreen->getTopScreen();
	}
	
	Screen* Screen::getBottomScreen()
	{
		if(parentScreen == nullptr)
		{
			return this;
		}
		return parentScreen->getBottomScreen();
	}
	
	Screen* Screen::getRootScreen()
	{
		Screen* bottomScreen = getBottomScreen();
		if(bottomScreen->screenManager == nullptr)
		{
			return bottomScreen;
		}
		return bottomScreen->screenManager->getRootScreen();
	}
	
	bool Screen::isOnTop()
	{
		if(childScreen!=nullptr)
		{
			return false;
		}
		else if(screenManager!=nullptr)
		{
			size_t index = screenManager->screens.indexOf(this);
			if(index == ArrayList<Screen*>::NOT_FOUND)
			{
				throw IllegalStateException("Screen is not contained in its ScreenManager");
			}
			else
			{
				if(index != (screenManager->screens.size() - 1))
				{
					return false;
				}
				return screenManager->isOnTop();
			}
		}
		else if(window != nullptr)
		{
			return true;
		}
		return false;
	}
	
	bool Screen::isPresenting() const
	{
		if(overlayData.action == TRANSITION_SHOW)
		{
			return true;
		}
		return false;
	}
	
	bool Screen::isDismissing() const
	{
		if(overlayData.action == TRANSITION_HIDE)
		{
			return true;
		}
		return false;
	}
	
	void Screen::setParentScreenDrawingEnabled(bool enabled)
	{
		drawsParent = enabled;
	}
	
	bool Screen::isParentScreenDrawingEnabled() const
	{
		return drawsParent;
	}

	void Screen::setParentScreenUpdatingEnabled(bool enabled)
	{
		updatesParent = enabled;
	}

	bool Screen::isParentScreenUpdatingEnabled() const
	{
		return updatesParent;
	}
	
	Window* Screen::getWindow() const
	{
		return window;
	}

#define MOUSEBUTTONS_COUNT 4
	
	size_t Screen::getTouchDataIndex(ArrayList<Screen::MouseTouchData>& touches, unsigned int touchID)
	{
		for(size_t touches_size=touches.size(), i=0; i<touches_size; i++)
		{
			if(touches[i].touchID==touchID)
			{
				return i;
			}
		}
		return ArrayList<Screen::MouseTouchData>::NOT_FOUND;
	}
	
	ArrayList<unsigned int> Screen::getUnlistedTouchIDs(ArrayList<Screen::MouseTouchData>& touches, ArrayList<unsigned int>& touchIDs)
	{
		ArrayList<unsigned int> unlisted;
		for(size_t touches_size=touches.size(), i=0; i<touches_size; i++)
		{
			MouseTouchData& touchData = touches.get(i);
			bool found = false;
			for(size_t j=0; j<touchIDs.size(); j++)
			{
				if(touchIDs.get(i) == touchData.touchID)
				{
					found = true;
					j = touchIDs.size();
				}
			}

			if(!found)
			{
				unlisted.add(touchData.touchID);
			}
		}
		return unlisted;
	}
	
	void Screen::updateElementMouse(ApplicationData appData)
	{
		Window* window = appData.getWindow();
		ScreenElement* element = getElement();
		TransformD mouseTransform = appData.getTransform().getInverse();
		
		ArrayList<unsigned int> mouseIndexes;
		unsigned int mouseCount = Mouse::getTotalMouseInstances(window);
		mouseIndexes.resize((size_t)mouseCount);
		for(unsigned int i=0; i<mouseCount; i++)
		{
			mouseIndexes[i] = i;
		}
		
		ArrayList<unsigned int> unlistedIDs = getUnlistedTouchIDs(currentTouches, mouseIndexes);
		for(size_t unlistedIDs_size=unlistedIDs.size(), i=0; i<unlistedIDs_size; i++)
		{
			unsigned int touchID = unlistedIDs.get(i);
			size_t touch_index = getTouchDataIndex(currentTouches, touchID);
			if(touch_index!=ArrayList<Screen::MouseTouchData>::NOT_FOUND)
			{
				MouseTouchData& touchData = currentTouches[touch_index];
				element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHCANCEL, touchData.touchID, appData, touchData.pos, true));
				currentTouches.remove(touch_index);
			}
		}
		
		for(size_t mouseIndexes_size=mouseIndexes.size(), i=0; i<mouseIndexes_size; i++)
		{
			unsigned int mouseIndex = mouseIndexes.get(i);
			size_t touch_index = getTouchDataIndex(currentTouches, mouseIndex);
			MouseTouchData* touchData_ptr = nullptr;
			if(touch_index==ArrayList<Screen::MouseTouchData>::NOT_FOUND)
			{
				MouseTouchData touchData;
				touchData.touchID = mouseIndex;
				touchData.pos = mouseTransform.transform(Mouse::getPosition(window, mouseIndex));
				//TODO add mouse connect handler
				element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHMOVE, touchData.touchID, appData, touchData.pos, true));
				for(size_t j=0; j<MOUSEBUTTONS_COUNT; j++)
				{
					touchData.state[j] = Mouse::isButtonPressed(window, mouseIndex, (Mouse::Button)j);
					if(touchData.state[j] && j!=0)
					{
						//TODO include mouse buttons
						element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHDOWN, touchData.touchID, appData, touchData.pos, true));
					}
				}
				currentTouches.add(touchData);
				touchData_ptr = &currentTouches[currentTouches.size()-1];
			}
			else
			{
				touchData_ptr = &currentTouches[touch_index];
				Vector2d mousepos = mouseTransform.transform(Mouse::getPosition(window, touchData_ptr->touchID));
				if(mousepos!=touchData_ptr->pos)
				{
					//mouse moved
					element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHMOVE, touchData_ptr->touchID, appData, mousepos, true));
				}
				touchData_ptr->pos = mousepos;
			}
			
			MouseTouchData& touchData = *touchData_ptr;
			
			for(size_t j=1; j<MOUSEBUTTONS_COUNT; j++)
			{
				bool state = Mouse::isButtonPressed(window, touchData.touchID, (Mouse::Button)j);
				if(touchData.state[j]!=state)
				{
					//TODO include mouse buttons
					if(state)
					{
						element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHDOWN, touchData.touchID, appData, touchData.pos, true));
					}
					else
					{
						element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHUP, touchData.touchID, appData, touchData.pos, true));
					}
					touchData.state[j] = state;
				}
			}
		}
	}
	
	void Screen::updateElementTouch(ApplicationData appData)
	{
		Window* window = appData.getWindow();
		ScreenElement* element = getElement();
		TransformD touchTransform = appData.getTransform().getInverse();

		ArrayList<unsigned int> touchIDs = Multitouch::getTouchIDs(window);

		ArrayList<unsigned int> unlistedIDs = getUnlistedTouchIDs(currentTouches, touchIDs);
		for(size_t unlistedIDs_size=unlistedIDs.size(), i=0; i<unlistedIDs_size; i++)
		{
			unsigned int touchID = unlistedIDs.get(i);
			size_t touch_index = getTouchDataIndex(currentTouches, touchID);
			if(touch_index!=ArrayList<Screen::MouseTouchData>::NOT_FOUND)
			{
				MouseTouchData& touchData = currentTouches[touch_index];
				element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHUP, touchData.touchID, appData, touchData.pos, false));
				currentTouches.remove(touch_index);
			}
		}
		
		for(size_t touchIDs_size=touchIDs.size(), i=0; i<touchIDs_size; i++)
		{
			unsigned int touchID = touchIDs.get(i);
			size_t touch_index = getTouchDataIndex(currentTouches, touchID);
			if(touch_index==ArrayList<Screen::MouseTouchData>::NOT_FOUND)
			{
				MouseTouchData touchData;
				touchData.touchID = touchID;
				touchData.pos = touchTransform.transform(Multitouch::getPosition(window, touchID));
				for(size_t j=0; j<MOUSEBUTTONS_COUNT; j++)
				{
					touchData.state[j] = false;
				}
				touchData.state[Mouse::BUTTON_LEFT] = true; //touch events are detected as the left mouse button being pressed
				element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHDOWN, touchData.touchID, appData, touchData.pos, false));
				currentTouches.add(touchData);
			}
			else
			{
				MouseTouchData& touchData = currentTouches[touch_index];
				Vector2d touchpos = touchTransform.transform(Multitouch::getPosition(window, touchData.touchID));
				if(touchpos!=touchData.pos)
				{
					//touch moved
					element->sendTouchEvent(ScreenElement::TouchEvent(ScreenElement::TouchEvent::EVENTTYPE_TOUCHMOVE, touchData.touchID, appData, touchpos, false));
				}
				touchData.pos = touchpos;
			}
		}
	}
}
