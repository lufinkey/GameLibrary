
#include <GameLibrary/Screen/Screen.hpp>
#include <GameLibrary/Screen/ScreenManager.hpp>
#include <GameLibrary/Screen/Transition/PopoverTransition.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>
#include <GameLibrary/Exception/Screen/ScreenNavigationException.hpp>

namespace fgl
{
	const Transition* const Screen::defaultPresentationTransition = new PopoverTransition(PopoverTransition::POPOVER_UP);
	
	void Screen::updateFrame(Window* window) {
		if(window != nullptr) {
			Viewport* viewport = window->getViewport();
			Vector2d size;
			if(viewport != nullptr) {
				if(viewport->matchesWindow()) {
					size = (Vector2d)window->getSize();
				}
				else {
					size = viewport->getSize();
				}
			}
			else {
				size = (Vector2d)window->getSize();
			}
			auto elementFrame = element->getFrame();
			if(elementFrame.x != 0 || elementFrame.y != 0 || elementFrame.width != size.x || elementFrame.height != size.y) {
				Vector2d oldSize = elementFrame.getSize();
				element->setFrame(RectangleD(0,0,size.x,size.y));
				onSizeChange(oldSize, size);
			}
		}
		for(auto& container : childScreens) {
			container.screen->updateFrame(window);
		}
	}
	
	void Screen::setWindow(Window* window_arg) {
		auto oldWindow = window;
		if(window != window_arg) {
			if(oldWindow != nullptr) {
				element->handleRemoveFromWindow(oldWindow);
			}
			
			window = window_arg;
			updateFrame(window);
			
			element->handleAddToWindow(window);
			
			for(auto& container : childScreens) {
				container.screen->setWindow(window);
			}
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
	
	bool Screen::TransitionData_applyProgress(ApplicationData& appData, Screen::TransitionData&data)
	{
		if(data.action != TRANSITION_NONE)
		{
			TransitionData_checkInitialization(appData, data);
			long long currentTime = appData.getTime().getMilliseconds();
			long long timeDif = currentTime - data.startTime;
			data.progress = (double)(((double)timeDif) / ((double)data.duration));
			if(data.progress >= 1)
			{
				return true;
			}
		}
		return false;
	}
	
	std::function<void()> Screen::TransitionData_checkFinished(ApplicationData& appData, Screen::TransitionData& data)
	{
		//apply any progress to the presenting transition
		std::function<void()> completion = data.completion;
		if(TransitionData_applyProgress(appData, data))
		{
			return [=]{
				if(completion)
				{
					completion();
				}
			};
		}
		return nullptr;
	}

	void Screen::addTransition(const String& name, float zLayer, Screen* screen, Screen* transitionScreen, TransitionAction action, const Transition* transition, long long duration, const std::function<void()>& completion)
	{
		if(isTransitionAnimating(name))
		{
			throw fgl::IllegalArgumentException("name", "transition is already animating");
		}
		for(auto& transitionData : transitions)
		{
			if(transitionData.screen==screen || transitionData.transitionScreen==screen)
			{
				throw fgl::IllegalArgumentException("screen", "is already transitioning");
			}
			else if(transitionData.screen==transitionScreen || transitionData.transitionScreen==transitionScreen)
			{
				throw fgl::IllegalArgumentException("transitionScreen", "is already transitioning");
			}
		}
		if(transition==nullptr || duration==0)
		{
			if(completion)
			{
				completion();
			}
		}
		else
		{
			TransitionData transitionData;
			TransitionData_clear(transitionData);
			transitionData.name = name;
			transitionData.zLayer = zLayer;
			TransitionData_begin(transitionData, screen, transitionScreen, action, transition, duration, completion);
			transitions.add(transitionData);
		}
	}

	bool Screen::isTransitionAnimating(const String& name) const
	{
		for(auto& transitionData : transitions)
		{
			if(transitionData.name==name)
			{
				return true;
			}
		}
		return false;
	}

	void Screen::addChildScreen(Screen* screen, float zLayer, bool visible)
	{
		if(screen->parentScreen!=nullptr)
		{
			throw IllegalArgumentException("screen", "cannot add a child Screen that already has a parent Screen");
		}
		screen->parentScreen = this;
		screen->setWindow(window);
		size_t screenIndex = -1;
		for(size_t i=0; i<childScreens.size(); i++)
		{
			auto& container = childScreens[i];
			if(container.zLayer > zLayer)
			{
				screenIndex = i;
				break;
			}
		}
		if(screenIndex == -1)
		{
			screenIndex = childScreens.size();
		}
		ChildScreenContainer container;
		container.screen = screen;
		container.zLayer = zLayer;
		container.visible = visible;
		childScreens.add(container);
	}

	void Screen::removeChildScreen(Screen* screen)
	{
		if(screen->parentScreen!=this)
		{
			return;
		}
		screen->parentScreen = nullptr;
		screen->setWindow(nullptr);
		size_t screenIndex = childScreens.indexWhere([&](const ChildScreenContainer& container) -> bool {
			if(container.screen == screen)
			{
				return true;
			}
			return false;
		});
		if(screenIndex!=-1)
		{
			childScreens.remove(screenIndex);
		}
	}

	void Screen::setChildScreenVisible(Screen* screen, bool visible)
	{
		for(auto& container : childScreens)
		{
			if(container.screen == screen)
			{
				container.visible = visible;
				return;
			}
		}
		throw IllegalArgumentException("screen", "is not a child of this Screen");
	}

	void Screen::handleFirstShowing()
	{
		if(!isshown)
		{
			isshown = true;
			if(window!=nullptr)
			{
				updateFrame(window);
			}
			if(window != nullptr && parentScreen == nullptr && screenManager == nullptr)
			{
				onWillAppear(nullptr);
				onDidAppear(nullptr);
			}
		}
	}
	
	Screen::Screen(Window* window_arg)
	{
		element = new ScreenElement();
		screenManager = nullptr;
		parentScreen = nullptr;
		presentedScreen = nullptr;

		drawingOverlayTransition = false;
		isshown = false;
		drawsParent = false;
		updatesParent = false;

		window = window_arg;
		if(window != nullptr) {
			Viewport* viewport = window->getViewport();
			if(viewport != nullptr) {
				const Vector2d& size = viewport->getSize();
				element->setFrame(RectangleD(0,0,size.x,size.y));
			}
		}
	}
	
	Screen::Screen() : Screen(nullptr)
	{
		//
	}
	
	Screen::~Screen()
	{
		delete element;
	}
	
	void Screen::onSizeChange(const Vector2d& oldFrame, const Vector2d& newFrame)
	{
		//Open for implementation
	}
	
	void Screen::onWillAppear(const Transition* transition)
	{
		for(auto& container : childScreens)
		{
			if(container.zLayer < 0.5)
			{
				container.screen->onWillAppear(transition);
			}
		}
		//Open for implementation
	}
	
	void Screen::onDidAppear(const Transition* transition)
	{
		for(auto& container : childScreens)
		{
			if(container.zLayer < 0.5)
			{
				container.screen->onDidAppear(transition);
			}
		}
		//Open for implementation
	}
	
	void Screen::onWillDisappear(const Transition* transition)
	{
		for(auto& container : childScreens)
		{
			if(container.zLayer < 0.5)
			{
				container.screen->onWillDisappear(transition);
			}
		}
		//Open for implementation
	}
	
	void Screen::onDidDisappear(const Transition* transition)
	{
		for(auto& container : childScreens)
		{
			if(container.zLayer < 0.5)
			{
				container.screen->onDidDisappear(transition);
			}
		}
		//Open for implementation
	}
	
	void Screen::update(ApplicationData appData)
	{
		handleFirstShowing();
		element->layoutChildElementsIfNeeded();

		ArrayList<std::function<void()>> transitionCompletions;
		for(size_t i=(transitions.size()-1); i!=-1; i--)
		{
			auto& transitionData = transitions[i];
			if(auto transitionFinish = TransitionData_checkFinished(appData, transitionData))
			{
				transitionCompletions.add(transitionFinish);
				transitions.remove(i);
			}
		}
		
		//only update top child screen
		Screen* updateCaller = nullptr;
		float updateCallerZLayer = 0;
		if(childScreens.size() > 0)
		{
			auto& topContainer = childScreens[childScreens.size()-1];
			updateCaller = topContainer.screen;
			updateCallerZLayer = topContainer.zLayer;
		}
		
		for(auto& completion : transitionCompletions)
		{
			completion();
		}

		if(updateCaller==nullptr || updateCaller->updatesParent || updateCallerZLayer < 0.5)
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
		
		for(auto& transitionData : transitions)
		{
			TransitionData_checkInitialization(appData, transitionData);
		}
		
		updateFrame(window);
	}
	
	void Screen::onUpdate(const ApplicationData& appData)
	{
		//Open for implementation
	}
	
	void Screen::drawElements(const ApplicationData& appData, Graphics graphics) const
	{
		if(element != nullptr)
		{
			element->draw(appData, graphics);
		}
		onDraw(appData, graphics);
	}
	
	void Screen::drawTransition(const TransitionData* transitionData, const ApplicationData& appData, Graphics graphics) const
	{
		double progress = transitionData->progress;

		if(transitionData->action == TRANSITION_HIDE)
		{
			progress = 1 - progress;
		}

		transitionData->transition->draw(appData, graphics, progress, transitionData->screen, transitionData->transitionScreen);
	}

	void Screen::drawChildScreen(Screen* childScreen, const ApplicationData& appData, Graphics graphics) const
	{
		auto size = childScreen->getSize();
		auto parentSize = getSize();
		double xOff = (size.x - parentSize.x)/2;
		double yOff = (size.y - parentSize.y)/2;
		graphics.translate(xOff, yOff);
		childScreen->draw(appData, graphics);
	}
	
	void Screen::draw(ApplicationData appData, Graphics graphics) const
	{
		if(drawingOverlayTransition)
		{
			drawElements(appData, graphics);
		}
		else
		{
			struct DrawOperation
			{
				float zLayer;
				std::function<void()> func;
			};
			ArrayList<DrawOperation> drawOperations;
			drawOperations.reserve(transitions.size()+childScreens.size());

			bool shouldDrawElements = true;
			//add transition drawing operations
			for(auto& transitionData : transitions)
			{
				if(transitionData.screen==this)
				{
					shouldDrawElements = false;
					DrawOperation op;
					op.zLayer = transitionData.zLayer;
					op.func = [&]{
						drawingOverlayTransition = true;
						drawTransition(&transitionData, appData, graphics);
						drawingOverlayTransition = false;
					};
					drawOperations.add(op);
				}
				else
				{
					if(!transitionData.screen->drawsParent && !transitionData.transitionScreen->drawsParent)
					{
						shouldDrawElements = false;
					}
					DrawOperation op;
					op.zLayer = transitionData.zLayer;
					op.func = [&]{
						drawTransition(&transitionData, appData, graphics);
					};
					drawOperations.add(op);
				}
			}
			//add child screen drawing operations
			for(auto& container : childScreens)
			{
				//don't draw this child screen if it isn't visible
				if(!container.visible)
				{
					continue;
				}
				//don't draw this child screen if it is transitioning
				bool transitioning = false;
				for(auto& transitionData : transitions)
				{
					if(transitionData.screen==container.screen || transitionData.transitionScreen==container.screen)
					{
						transitioning = true;
						break;
					}
				}
				if(transitioning)
				{
					continue;
				}

				//find the index in drawOperations where this screen should be drawn
				size_t opIndex = -1;
				for(size_t i=0; i<drawOperations.size(); i++)
				{
					auto& cmpOp = drawOperations[i];
					if(cmpOp.zLayer > container.zLayer)
					{
						opIndex = i;
						break;
					}
				}
				if(opIndex==-1)
				{
					opIndex = drawOperations.size();
				}

				if(!container.screen->drawsParent)
				{
					shouldDrawElements = false;
				}

				DrawOperation op;
				op.zLayer = container.zLayer;
				op.func = [&]{
					drawChildScreen(container.screen, appData, graphics);
				};
				drawOperations.add(opIndex, op);
			}
			//add elements drawing operation
			if(shouldDrawElements)
			{
				size_t opIndex = -1;
				for(size_t i=0; i<drawOperations.size(); i++)
				{
					auto& cmpOp = drawOperations[i];
					if(cmpOp.zLayer >= 0.5)
					{
						opIndex = i;
						break;
					}
				}
				if(opIndex==-1)
				{
					opIndex = drawOperations.size();
				}

				DrawOperation op;
				op.zLayer = 0.5;
				op.func = [&]{
					drawElements(appData, graphics);
				};
				drawOperations.add(opIndex, op);
			}

			//perform drawing operations
			for(auto& op : drawOperations)
			{
				op.func();
			}
		}
	}
	
	void Screen::onDraw(const ApplicationData& appData, Graphics graphics) const
	{
		//Open for implementation
	}
	
	Vector2d Screen::getSize() const
	{
		if(window!=nullptr) {
			auto viewport = window->getViewport();
			if(viewport!=nullptr) {
				if(viewport->matchesWindow()) {
					return (fgl::Vector2d)window->getSize();
				}
				else {
					return viewport->getSize();
				}
			}
			else {
				return (fgl::Vector2d)window->getSize();
			}
		}
		return element->getFrame().getSize();
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
	
	void Screen::presentScreen(Screen* screen, const Transition* transition, long long duration, const std::function<void()>& oncompletion, const std::function<void()>& ondismissal)
	{
		handleFirstShowing();
		
		if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be a negative value");
		}
		else if(screen == nullptr)
		{
			throw IllegalArgumentException("screen", "cannot be null");
		}
		else if(screen->parentScreen != nullptr)
		{
			throw IllegalArgumentException("screen", "already presenting on another Screen");
		}
		else if(screen->screenManager != nullptr)
		{
			throw IllegalArgumentException("screen", "already displaying in a ScreenManager");
		}
		else if(screen->window != nullptr)
		{
			throw IllegalArgumentException("screen", "already belongs to a Window");
		}
		else if(presentedScreen != nullptr)
		{
			throw ScreenNavigationException("Cannot present Screen on top of a screen that already has a presented screen");
		}
		else if(isTransitionAnimating("present") || isTransitionAnimating("dismiss"))
		{
			throw ScreenNavigationException("Cannot present Screen on top of a screen that is presenting or dismissing");
		}
		else
		{
			Screen* topScreen = screen->getTopScreenInHeirarchy();
			bool visible = isOnTop();

			presentedScreen = screen;
			presentedScreenDismissCallback = ondismissal;
			addChildScreen(screen, 1.0);

			if(visible)
			{
				onWillDisappear(transition);
				topScreen->onWillAppear(transition);
			}
			addTransition("present", 1.0, this, screen, TRANSITION_SHOW, transition, duration, [=]{
				if(visible)
				{
					onDidDisappear(transition);
					topScreen->onDidAppear(transition);
				}
				if(oncompletion)
				{
					oncompletion();
				}
			});
		}
	}
	
	void Screen::presentScreen(Screen* screen, const std::function<void()>& oncompletion, const std::function<void()>& ondismissal)
	{
		if(screen == nullptr)
		{
			throw IllegalArgumentException("screen", "cannot be null");
		}
		presentScreen(screen, screen->getDefaultPresentationTransition(), screen->getDefaultDismissalDuration(), oncompletion, ondismissal);
	}
	
	void Screen::dismissPresentedScreen(const Transition* transition, long long duration, const std::function<void()>& oncompletion)
	{
		if(duration < 0)
		{
			throw IllegalArgumentException("duration", "cannot be a negative value");
		}
		else if(presentedScreen == nullptr)
		{
			throw ScreenNavigationException("No Screen is being presented that can be dismissed");
		}
		else if(isTransitionAnimating("dismiss"))
		{
			throw ScreenNavigationException("Cannot dismiss a Screen that is already dismissing");
		}
		else if(isTransitionAnimating("present"))
		{
			throw ScreenNavigationException("Cannot dismiss a Screen that is in the transition of presenting");
		}
		else
		{
			Screen* topScreen = presentedScreen->getTopScreenInHeirarchy();
			bool visible = topScreen->isOnTop();

			if(visible)
			{
				topScreen->onWillDisappear(transition);
				onWillAppear(transition);
			}
			auto dismissHandler = presentedScreenDismissCallback;
			auto prevPresentedScreen = presentedScreen;
			presentedScreenDismissCallback = nullptr;
			presentedScreen = nullptr;
			removeChildScreen(prevPresentedScreen);
			addTransition("dismiss", 1.0, this, prevPresentedScreen, TRANSITION_HIDE, transition, duration, [=] {
				if(visible)
				{
					topScreen->onDidDisappear(transition);
					onDidAppear(transition);
				}
				if(oncompletion)
				{
					oncompletion();
				}
				if(dismissHandler)
				{
					dismissHandler();
				}
			});
		}
	}
	
	void Screen::dismissPresentedScreen(const std::function<void()>& completion)
	{
		if(presentedScreen == nullptr)
		{
			throw ScreenNavigationException("No Screen is being presented that can be dismissed");
		}
		dismissPresentedScreen(presentedScreen->getDefaultDismissalTransition(), presentedScreen->getDefaultDismissalDuration(), completion);
	}
	
	ScreenElement* Screen::getElement() const
	{
		return element;
	}
	
	ScreenManager* Screen::getScreenManager() const
	{
		return screenManager;
	}

	Screen* Screen::getPresentedScreen() const
	{
		return presentedScreen;
	}

	Screen* Screen::getPresentingScreen() const
	{
		if(parentScreen!=nullptr)
		{
			if(parentScreen->presentedScreen==this)
			{
				return parentScreen;
			}
		}
		return nullptr;
	}
	
	Screen* Screen::getParentScreen() const
	{
		return parentScreen;
	}
	
	ArrayList<Screen*> Screen::getChildScreens() const
	{
		ArrayList<Screen*> children;
		children.reserve(childScreens.size());
		for(auto& container : childScreens)
		{
			children.add(container.screen);
		}
		return children;
	}
	
	Screen* Screen::getTopScreenInHeirarchy() const
	{
		Screen* overlayedScreen = nullptr;
		float lastZLayer = 0.5;
		for(auto& container : childScreens)
		{
			if(container.zLayer >= lastZLayer)
			{
				overlayedScreen = container.screen;
			}
		}
		if(overlayedScreen==nullptr)
		{
			return (Screen*)this;
		}
		return overlayedScreen->getTopScreenInHeirarchy();
	}
	
	Screen* Screen::getBottomScreenInHeirarchy() const
	{
		if(parentScreen == nullptr)
		{
			return (Screen*)this;
		}
		return parentScreen->getBottomScreenInHeirarchy();
	}
	
	bool Screen::isOnTop()
	{
		for(auto& container : childScreens)
		{
			if(container.zLayer >= 0.5)
			{
				return false;
			}
		}
		if(screenManager!=nullptr)
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
		return isTransitionAnimating("present");
	}
	
	bool Screen::isDismissing() const
	{
		return isTransitionAnimating("dismiss");
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
				touchData.pos = Mouse::getPosition(window, mouseIndex);
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
				Vector2d mousepos = Mouse::getPosition(window, touchData_ptr->touchID);
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
				touchData.pos = Multitouch::getPosition(window, touchID);
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
				Vector2d touchpos = Multitouch::getPosition(window, touchData.touchID);
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
