
#include <GameLibrary/Screen/UI/TouchElement.hpp>

namespace fgl
{
	TouchElement::TouchElement() : TouchElement(RectangleD(0, 0, 0, 0))
	{
		//
	}
	
	TouchElement::TouchElement(const RectangleD& frame) : ScreenElement(frame),
		touchEnabled(true)
	{
		//
	}
	
	void TouchElement::update(ApplicationData appData)
	{
		ScreenElement::update(appData);
		if(!isVisibleInHeirarchy() || !touchEnabled)
		{
			for(size_t i=(touches.size()-1); i!=-1; i--)
			{
				auto& touchEvent = touches[i].lastEvent;
				cancelTouch(touchEvent.withAppData(appData));
			}
		}
		unsigned int mouseCount = Mouse::getTotalMouseInstances(appData.getWindow());
		for(unsigned int i=0; i<mouseCount; i++)
		{
			Vector2d position = appData.getTransform().getInverse().transform(Mouse::getPosition(appData.getWindow(), i));
			if(isPointInside(position))
			{
				size_t index = enteredMouses.indexOf(i);
				if(index==-1)
				{
					enteredMouses.add(i);
					onMouseEnter(i);
				}
			}
			else
			{
				size_t index = enteredMouses.indexOf(i);
				if(index!=-1)
				{
					enteredMouses.remove(index);
					onMouseLeave(i);
				}
			}
		}
	}
	
	void TouchElement::onMouseEnter(unsigned int mouseIndex) {}
	void TouchElement::onMouseLeave(unsigned int mouseIndex) {}
	void TouchElement::onTouchDown(const TouchEvent& evt) {}
	bool TouchElement::onTouchMove(const TouchEvent& evt) { return true; }
	void TouchElement::onTouchUpInside(const TouchEvent& evt) {}
	void TouchElement::onTouchUpOutside(const TouchEvent& evt) {}
	void TouchElement::onTouchCancel(const TouchEvent& evt) {}
	
	void TouchElement::setTouchEnabled(bool toggle)
	{
		touchEnabled = toggle;
	}
	
	bool TouchElement::isTouchEnabled() const
	{
		return touchEnabled;
	}
	
	const ArrayList<unsigned int> TouchElement::getHoveredMouseIndexes() const
	{
		return enteredMouses;
	}
	
	
	size_t TouchElement::getTouchIndex(unsigned int touchID, bool mouse) const
	{
		for(size_t touches_size=touches.size(), i=0; i<touches_size; i++)
		{
			const TouchData& touchData = touches[i];
			if(touchData.lastEvent.getTouchID()==touchID && touchData.lastEvent.isMouseEvent()==mouse)
			{
				return i;
			}
		}
		return -1;
	}

	void TouchElement::addTouch(const TouchEvent& touchEvent)
	{
		size_t touchIndex = getTouchIndex(touchEvent.getTouchID(), touchEvent.isMouseEvent());
		if(touchIndex==-1)
		{
			TouchData touchData = { .lastEvent=touchEvent, .inside=true };
			touches.add(touchData);
		}
		else
		{
			TouchData& touchData = touches[touchIndex];
			touchData.lastEvent = touchEvent;
			touchData.inside = true;
		}
	}
	
	void TouchElement::removeTouch(unsigned int touchID, bool mouse)
	{
		size_t touchIndex = getTouchIndex(touchID, mouse);
		if(touchIndex!=-1)
		{
			touches.remove(touchIndex);
		}
	}
	
	void TouchElement::cancelTouch(const TouchEvent& touchEvent)
	{
		size_t touchIndex = getTouchIndex(touchEvent.getTouchID(), touchEvent.isMouseEvent());
		if(touchIndex!=-1)
		{
			TouchData touchData = touches[touchIndex];
			removeTouch(touchData.lastEvent.getTouchID(), touchData.lastEvent.isMouseEvent());
			/*if(touchData.lastEvent.isMouseEvent())
			{
				if(touchData.inside)
				{
					onMouseLeave(touchEvent.getTouchID());
				}
			}*/
			onTouchCancel(touchEvent);
		}
	}

	bool TouchElement::isPointInside(const Vector2d& point) const
	{
		return getFrame().contains(point);
	}


	bool TouchElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		if(!touchEnabled || !isVisibleInHeirarchy())
		{
			cancelTouch(touchEvent);
			return false;
		}
		switch(touchEvent.getEventType())
		{
			case TouchEvent::EVENTTYPE_TOUCHDOWN:
			if(isPointInside(touchEvent.getPosition()))
			{
				addTouch(touchEvent);
				onTouchDown(touchEvent);
				return true;
			}
			return false;

			case TouchEvent::EVENTTYPE_TOUCHMOVE:
			{
				size_t touchIndex = getTouchIndex(touchEvent.getTouchID(), touchEvent.isMouseEvent());
				if(touchIndex==-1)
				{
					return false;
				}
				else
				{
					TouchData& touchData = touches[touchIndex];
					touchData.lastEvent = touchEvent;
					bool shouldContinueTracking = onTouchMove(touchEvent);
					if(shouldContinueTracking)
					{
						touchData.inside = isPointInside(touchEvent.getPosition());
						return true;
					}
					else
					{
						cancelTouch(touchEvent);
						return false;
					}
				}
			}

			case TouchEvent::EVENTTYPE_TOUCHUP:
			{
				size_t touchIndex = getTouchIndex(touchEvent.getTouchID(), touchEvent.isMouseEvent());
				if(touchIndex==-1)
				{
					return false;
				}
				else
				{
					bool inside = isPointInside(touchEvent.getPosition());
					removeTouch(touchEvent.getTouchID(), touchEvent.isMouseEvent());
					if(inside)
					{
						onTouchUpInside(touchEvent);
					}
					else
					{
						onTouchUpOutside(touchEvent);
					}
					return true;
				}
			}

			case TouchEvent::EVENTTYPE_TOUCHCANCEL:
			{
				cancelTouch(touchEvent);
				return false;
			}
		}
		return false;
	}

	void TouchElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		cancelTouch(touchEvent);
	}
}
