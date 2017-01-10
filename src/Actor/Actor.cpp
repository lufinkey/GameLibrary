
#include <GameLibrary/Actor/Actor.hpp>
#include <GameLibrary/Input/Mouse.hpp>
#include <GameLibrary/Input/Multitouch.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>

namespace fgl
{
	ActorMouseEvent::ActorMouseEvent()
		: target(nullptr),
		eventType(EVENTTYPE_NONE),
		appData(ApplicationData(nullptr, nullptr, nullptr, TimeInterval(), TransformD(), 0)),
		mouseIndex(-1),
		multitouch(false)
	{
		//
	}
	
	ActorMouseEvent::ActorMouseEvent(Actor* target, const ActorMouseEvent::EventType& eventType, const ApplicationData& appData, unsigned int mouseIndex, bool multitouch)
		: target(target),
		eventType(eventType),
		appData(appData),
		mouseIndex(mouseIndex),
		multitouch(multitouch)
	{
		//
	}
	
	ActorMouseEvent::ActorMouseEvent(const ActorMouseEvent& event)
		: target(event.target),
		eventType(event.eventType),
		appData(event.appData),
		mouseIndex(event.mouseIndex),
		multitouch(event.multitouch)
	{
		//
	}
	
	ActorMouseEvent& ActorMouseEvent::operator=(const ActorMouseEvent& event)
	{
		target = event.target;
		eventType = event.eventType;
		appData = event.appData;
		mouseIndex = event.mouseIndex;
		multitouch = event.multitouch;
		return *this;
	}
	
	Actor* ActorMouseEvent::getTarget() const
	{
		return target;
	}
	
	const ActorMouseEvent::EventType& ActorMouseEvent::getEventType() const
	{
		return eventType;
	}
	
	const ApplicationData& ActorMouseEvent::getApplicationData() const
	{
		return appData;
	}
	
	unsigned int ActorMouseEvent::getMouseIndex() const
	{
		return mouseIndex;
	}
	
	bool ActorMouseEvent::isMultitouchEvent() const
	{
		return multitouch;
	}
	
	Actor::Actor() :
		x(0),
		y(0),
		width(0),
		height(0),
		prevx(0),
		prevy(0),
		clicked(false),
		prevclicked(false),
		mouseover(false),
		prevmouseover(false),
		didpress(false),
		didrelease(false),
		visible(true),
		mirroredHorizontal(false),
		mirroredVertical(false),
		frame_visible(false),
		mouseEventsEnabled(true),
		color(Color::WHITE),
		frame_color(Color::GREEN),
		rotation(0),
		alpha(1),
		scale(1)
	{
		//
	}
	
	Actor::~Actor()
	{
		//
	}
	
	void Actor::update(ApplicationData appData)
	{
		prevx = x;
		prevy = y;
		
		if(mouseEventsEnabled)
		{
			didpress = false;
			didrelease = false;
			prevclicked = clicked;
			prevmouseover = mouseover;

			if(Multitouch::isAvailable())
			{
				updateTouch(appData);
			}
			else
			{
				updateMouse(appData);
			}
		}
	}
	
	void Actor::draw(ApplicationData appData, Graphics graphics) const
	{
		//Open for implementation
	}
	
	RectangleD Actor::getFrame() const
	{
		return RectangleD(x, y, width, height);
	}
	
	void Actor::scaleToFit(const RectangleD&container)
	{
		if(width==0 || height==0)
		{
			double oldScale = scale;
			setScale(1);
			if(width==0 || height==0)
			{
				setScale(oldScale);
				x = container.x + (container.width/2);
				y = container.y + (container.height/2);
				return;
			}
		}
		if(container.width==0 || container.height==0)
		{
			setScale(0);
			x = container.x + (container.width/2);
			y = container.y + (container.height/2);
			return;
		}
		RectangleD currentFrame = getFrame();
		RectangleD oldFrame = currentFrame;
		currentFrame.scaleToFit(container);
		double ratio = currentFrame.width/oldFrame.width;
		setScale(getScale()*ratio);
		RectangleD newFrame = getFrame();
		x = container.x + ((container.width-newFrame.width)/2);
		y = container.y + ((container.height-newFrame.height)/2);
	}
	
	void Actor::scaleToFit(const Vector2d& size)
	{
		if(width==0 || height==0)
		{
			double oldScale = scale;
			setScale(1);
			if(width==0 || height==0)
			{
				setScale(oldScale);
				return;
			}
		}
		if(size.x==0 || size.y==0)
		{
			setScale(0);
			return;
		}
		RectangleD container(0,0,size.x,size.y);
		RectangleD currentFrame = getFrame();
		RectangleD oldFrame = currentFrame;
		currentFrame.scaleToFit(container);
		double ratio = currentFrame.width/oldFrame.width;
		setScale(getScale()*ratio);
	}
	
	double Actor::getWidth() const
	{
		return width;
	}
	
	double Actor::getHeight() const
	{
		return height;
	}
	
	double Actor::getPreviousX() const
	{
		return prevx;
	}
	
	double Actor::getPreviousY() const
	{
		return prevy;
	}
	
	void Actor::setVisible(bool toggle)
	{
		visible = toggle;
	}
	
	void Actor::setColor(const Color&c)
	{
		color = c;
	}
	
	void Actor::rotate(double degrees)
	{
		if(degrees!=0)
		{
			rotation += degrees;
			rotationMatrix.rotate(degrees,0,0);
			inverseRotationMatrix = rotationMatrix.getInverse();
			updateSize();
		}
	}
	
	void Actor::setRotation(double degrees)
	{
		if(degrees!=rotation)
		{
			rotation = degrees;
			rotationMatrix.reset();
			if(degrees!=0)
			{
				rotationMatrix.rotate(degrees,0,0);
			}
			inverseRotationMatrix = rotationMatrix.getInverse();
			updateSize();
		}
	}
	
	void Actor::setAlpha(double a)
	{
		alpha = a;
	}
	
	void Actor::setScale(double s)
	{
		scale = s;
		updateSize();
	}

	void Actor::setFrameVisible(bool toggle)
	{
		frame_visible = toggle;
	}
	
	void Actor::setFrameColor(const Color&color)
	{
		frame_color = color;
	}
	
	void Actor::setMirroredHorizontal(bool toggle)
	{
		mirroredHorizontal = toggle;
		updateSize();
	}
	
	void Actor::setMirroredVertical(bool toggle)
	{
		mirroredVertical = toggle;
		updateSize();
	}
	
	void Actor::setMouseEventsEnabled(bool enabled)
	{
		mouseEventsEnabled = enabled;
	}
	
	bool Actor::isVisible() const
	{
		return visible;
	}
	
	const Color& Actor::getColor() const
	{
		return color;
	}
	
	double Actor::getRotation() const
	{
		return rotation;
	}
	
	double Actor::getAlpha() const
	{
		return alpha;
	}
	
	double Actor::getScale() const
	{
		return scale;
	}

	bool Actor::isFrameVisible() const
	{
		return frame_visible;
	}
	
	const Color& Actor::getFrameColor() const
	{
		return frame_color;
	}
	
	bool Actor::isMirroredHorizontal() const
	{
		return mirroredHorizontal;
	}
	
	bool Actor::isMirroredVertical() const
	{
		return mirroredVertical;
	}
	
	bool Actor::areMouseEventsEnabled() const
	{
		return mouseEventsEnabled;
	}
	
	bool Actor::isMouseOver() const
	{
		return mouseover;
	}
	
	bool Actor::wasMouseOver() const
	{
		return prevmouseover;
	}
	
	bool Actor::isMousePressed() const
	{
		return clicked;
	}
	
	bool Actor::wasMousePressed() const
	{
		return prevclicked;
	}
	
	bool Actor::didMousePress() const
	{
		return didpress;
	}

	bool Actor::didMouseRelease() const
	{
		return didrelease;
	}

	void Actor::clearMouseState()
	{
		currentTouches.clear();
	}
	
	void Actor::updateSize()
	{
		width = 0;
		height = 0;
	}

	bool Actor::checkPointCollision(const Vector2d&point)
	{
		return false;
	}
	
	void Actor::onMousePress(const ActorMouseEvent& evt)
	{
		//Open for implementation
	}

	void Actor::onMouseRelease(const ActorMouseEvent& evt)
	{
		//Open for implementation
	}

	void Actor::onMouseEnter(const ActorMouseEvent& evt)
	{
		//Open for implementation
	}

	void Actor::onMouseLeave(const ActorMouseEvent& evt)
	{
		//Open for implementation
	}

	bool Actor::isTouchDataActive(unsigned int touchID)
	{
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			if(currentTouches.get(i).touchID == touchID)
			{
				return true;
			}
		}
		return false;
	}

	bool Actor::isTouchDataPressed(unsigned int touchID)
	{
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			MouseTouchData& touchData = currentTouches.get(i);
			if(touchData.touchID == touchID)
			{
				return touchData.pressed;
			}
		}
		return false;
	}

	void Actor::applyTouchData(unsigned int touchID, bool pressed)
	{
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			MouseTouchData& touchData = currentTouches.get(i);
			if(touchData.touchID == touchID)
			{
				touchData.pressed = pressed;
				return;
			}
		}
		MouseTouchData touchData;
		touchData.touchID = touchID;
		touchData.pressed = pressed;
		currentTouches.add(touchData);
	}

	void Actor::removeTouchData(unsigned int touchID)
	{
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			MouseTouchData& touchData = currentTouches.get(i);
			if(touchData.touchID == touchID)
			{
				currentTouches.remove(i);
				return;
			}
		}
	}

	Actor::MouseTouchData* Actor::getTouchData(unsigned int touchID)
	{
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			MouseTouchData& touchData = currentTouches.get(i);
			if(touchData.touchID == touchID)
			{
				return &touchData;
			}
		}
		return nullptr;
	}

	ArrayList<unsigned int> Actor::getDifTouchData(const ArrayList<unsigned int>& touchIDs)
	{
		ArrayList<unsigned int> unlisted;
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			MouseTouchData& touchData = currentTouches.get(i);
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
	
	void Actor::updateMouse(ApplicationData&appData)
	{
		Window* window = appData.getWindow();
		TransformD mouseTransform = appData.getTransform().getInverse();
		
		mouseover = false;
		clicked = false;
		ArrayList<ActorMouseEvent> mouseEventCalls;
		
		ArrayList<unsigned int> touchIDs;
		unsigned int mouseInstances = Mouse::getTotalMouseInstances(window);
		for(unsigned int i=0; i<mouseInstances; i++)
		{
			touchIDs.add(i);
		}
		
		ArrayList<unsigned int> unlistedIDs = getDifTouchData(touchIDs);
		for(unsigned int i = 0; i < unlistedIDs.size(); i++)
		{
			unsigned int touchID = unlistedIDs.get(i);
			MouseTouchData* touchData = getTouchData(touchID);
			if(touchData != nullptr)
			{
				removeTouchData(touchID);
				mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSERELEASE, appData, touchID, false));
			}
		}

		for(unsigned int i=0; i<touchIDs.size(); i++)
		{
			unsigned int touchID = touchIDs.get(i);
			Vector2d mousepos = Mouse::getPosition(window, touchID);
			Vector2d transformedPos = mouseTransform.transform(mousepos);
			if(checkPointCollision(transformedPos))
			{
				mouseover = true;
				if(isTouchDataPressed(touchID))
				{
					if(Mouse::isButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
					{
						applyTouchData(touchID, true);
						clicked = true;
					}
					else
					{
						applyTouchData(touchID, false);
						mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSERELEASE, appData, touchID, false));
					}
				}
				else if(isTouchDataActive(touchID))
				{
					if(Mouse::isButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
					{
						applyTouchData(touchID, true);
						clicked = true;
						if(!Mouse::wasButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
						{
							mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSEPRESS, appData, touchID, false));
						}
					}
					else
					{
						applyTouchData(touchID, false);
						if(Mouse::wasButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
						{
							mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSERELEASE, appData, touchID, false));
						}
					}
				}
				else
				{
					mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSEENTER, appData, touchID, false));
					if(Mouse::isButtonPressed(window, touchID, Mouse::BUTTON_LEFT) && !Mouse::wasButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
					{
						applyTouchData(touchID, true);
						clicked = true;
						mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSEPRESS, appData, touchID, false));
					}
					else
					{
						applyTouchData(touchID, false);
					}
				}
			}
			else
			{
				if(isTouchDataPressed(touchID))
				{
					mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSELEAVE, appData, touchID, false));
					if(!Mouse::isButtonPressed(window, touchID, Mouse::BUTTON_LEFT))
					{
						mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSERELEASE, appData, touchID, false));
					}
				}
				else if(isTouchDataActive(touchID))
				{
					mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSELEAVE, appData, touchID, false));
				}
				removeTouchData(touchID);
			}
		}

		callMouseEvents(appData, mouseEventCalls);
	}
	
	void Actor::updateTouch(ApplicationData&appData)
	{
		Window* window = appData.getWindow();
		TransformD mouseTransform = appData.getTransform().getInverse();
		
		mouseover = false;
		clicked = false;
		ArrayList<ActorMouseEvent> mouseEventCalls;
		
		ArrayList<unsigned int> touchIDs = Multitouch::getTouchIDs(window);
		
		ArrayList<unsigned int> unlistedIDs = getDifTouchData(touchIDs);
		for(unsigned int i = 0; i < unlistedIDs.size(); i++)
		{
			unsigned int touchID = unlistedIDs.get(i);
			MouseTouchData* touchData = getTouchData(touchID);
			if(touchData != nullptr)
			{
				removeTouchData(touchID);
				mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSERELEASE, appData, touchID, true));
			}
		}
		
		for(unsigned int i=0; i<touchIDs.size(); i++)
		{
			unsigned int touchID = touchIDs.get(i);
			Vector2d touchpos = Multitouch::getPosition(window, touchID);
			Vector2d transformedPos = mouseTransform.transform(touchpos);
			if(checkPointCollision(transformedPos))
			{
				mouseover = true;
				if(isTouchDataActive(touchID))
				{
					applyTouchData(touchID, true);
					clicked = true;
				}
				else
				{
					if(!Multitouch::wasTouchActive(window, touchID))
					{
						applyTouchData(touchID, true);
						clicked = true;
						mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSEPRESS, appData, touchID, true));
					}
					else
					{
						applyTouchData(touchID, true);
						clicked = true;
						mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSEENTER, appData, touchID, true));
					}
				}
			}
			else
			{
				if(isTouchDataActive(touchID))
				{
					applyTouchData(touchID, false);
					mouseEventCalls.add(ActorMouseEvent(this, ActorMouseEvent::EVENTTYPE_MOUSELEAVE, appData, touchID, true));
				}
				else
				{
					//applyTouchData(touchID, false);
				}
				removeTouchData(touchID);
			}
		}
		
		callMouseEvents(appData, mouseEventCalls);
	}

	void Actor::callMouseEvents(ApplicationData&appData, const ArrayList<ActorMouseEvent>& eventCallData)
	{
		bool didmousepress = false;
		bool didmouserelease = false;
		for(unsigned int i=0; i<eventCallData.size(); i++)
		{
			const ActorMouseEvent& eventData = eventCallData.get(i);
			switch(eventData.eventType)
			{
				case ActorMouseEvent::EVENTTYPE_NONE:
				break;
				
				case ActorMouseEvent::EVENTTYPE_MOUSEENTER:
				onMouseEnter(eventData);
				break;

				case ActorMouseEvent::EVENTTYPE_MOUSELEAVE:
				onMouseLeave(eventData);
				break;

				case ActorMouseEvent::EVENTTYPE_MOUSEPRESS:
				didmousepress = true;
				onMousePress(eventData);
				break;

				case ActorMouseEvent::EVENTTYPE_MOUSERELEASE:
				didmouserelease = true;
				onMouseRelease(eventData);
				break;
			}
		}
		
		didpress = didmousepress;
		didrelease = didmouserelease;
		
		/*unsigned int pressedTouches = 0;
		for(unsigned int i=0; i<currentTouches.size(); i++)
		{
			if(currentTouches.get(i).pressed)
			{
				pressedTouches++;
			}
		}
		if(pressedTouches == 0)
		{
			didpress = false;
		}
		else
		{
			didrelease = false;
		}*/
	}
}
