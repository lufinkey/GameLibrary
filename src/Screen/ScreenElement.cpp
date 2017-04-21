
#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>

namespace fgl
{
	void ScreenElement::setWindow(Window* win)
	{
		Window*oldWindow = window;

		if(oldWindow!=win)
		{
			window = nullptr;

			if(oldWindow!=nullptr)
			{
				for(size_t i=0; i<childElements.size(); i++)
				{
					childElements.get(i)->setWindow(nullptr);
				}
				
				onRemoveFromWindow(oldWindow);
			}
			
			window = win;
			
			if(win!=nullptr)
			{
				onAddToWindow(win);

				for(size_t i=0; i<childElements.size(); i++)
				{
					childElements.get(i)->setWindow(win);
				}
			}
		}
	}
	
	void ScreenElement::onRemoveFromWindow(Window* window)
	{
		//
	}
	
	void ScreenElement::onAddToWindow(Window* window)
	{
		//
	}
	
	void ScreenElement::autoLayoutFrame()
	{
		if(parentElement!=nullptr)
		{
			setFrame(autoLayoutMgr.calculateFrame(frame, parentElement->getFrame()));
		}
	}
	
	ScreenElement::ScreenElement() : ScreenElement(RectangleD(0, 0, 0, 0))
	{
		//
	}
	
	ScreenElement::ScreenElement(const RectangleD& frame)
		: window(nullptr),
		frame(frame),
		screen(nullptr),
		parentElement(nullptr),
		backgroundColor(Color::TRANSPARENT),
		borderWidth(0),
		borderColor(Color::BLACK),
		alpha(1.0),
		visible(true),
		clipsToFrame(false)
	{
		//
	}
	
	ScreenElement::~ScreenElement()
	{
		//
	}
	
	void ScreenElement::update(ApplicationData appData)
	{
		RectangleD frame = getFrame();
		appData.getTransform().translate(frame.x, frame.y);
		updateElements(appData);
	}
	
	void ScreenElement::updateElements(ApplicationData appData)
	{
		ArrayList<ScreenElement*> children = childElements;
		for(size_t i=0; i<children.size(); i++)
		{
			ScreenElement* element = children.get(i);
			element->update(appData);
		}
	}
	
	void ScreenElement::drawBackground(ApplicationData appData, Graphics graphics) const
	{
		if(!backgroundColor.equals(Color::TRANSPARENT))
		{
			RectangleD rect = getFrame();
			graphics.setColor(backgroundColor);
			graphics.fillRect(rect.x, rect.y, rect.width, rect.height);
		}
	}

	void ScreenElement::drawBorder(ApplicationData appData, Graphics graphics) const
	{
		if(borderWidth != 0 && !borderColor.equals(Color::TRANSPARENT))
		{
			RectangleD frame = getFrame();
			graphics.setColor(borderColor);
			if(borderWidth > 0)
			{
				graphics.fillRect(frame.x, frame.y, frame.width, (double)borderWidth);
				graphics.fillRect(frame.x, frame.y, (double)borderWidth, frame.height);
				graphics.fillRect(frame.x, frame.y+frame.height-(double)borderWidth, frame.width, (double)borderWidth);
				graphics.fillRect(frame.x+frame.width-(double)borderWidth, frame.y, (double)borderWidth, frame.height);
			}
			else
			{
				graphics.fillRect(frame.x-(double)-borderWidth, frame.y, frame.width+(double)(2*-borderWidth), (double)borderWidth);
				graphics.fillRect(frame.x, frame.y-(double)-borderWidth, (double)borderWidth, frame.height+(double)(2*-borderWidth));
				graphics.fillRect(frame.x-(double)-borderWidth, frame.y+frame.height-(double)borderWidth, frame.width+(double)(2*-borderWidth), (double)borderWidth);
				graphics.fillRect(frame.x+frame.width-(double)borderWidth, frame.y-(double)-borderWidth, (double)borderWidth, frame.height+(double)(2*-borderWidth));
			}
		}
	}
	
	void ScreenElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		//Open for implementation
	}
	
	void ScreenElement::drawElements(ApplicationData appData, Graphics graphics) const
	{
		graphics.translate(frame.x, frame.y);
		ArrayList<ScreenElement*> children = childElements;
		for(auto element : children)
		{
			element->draw(appData, graphics);
		}
	}
	
	void ScreenElement::draw(ApplicationData appData, Graphics graphics) const
	{
		if(visible)
		{
			RectangleD frame = getFrame();
			if(clipsToFrame)
			{
				graphics.clip(frame);
			}
			graphics.compositeAlpha(alpha);
			drawBackground(appData, graphics);
			drawMain(appData, graphics);
			drawElements(appData, graphics);
			drawBorder(appData, graphics);
		}
	}
	
	void ScreenElement::setFrame(const RectangleD& frame_arg)
	{
		frame = frame_arg;
		layoutChildElements();
	}
	
	void ScreenElement::layoutChildElements()
	{
		for(size_t childElements_size=childElements.size(), i=0; i<childElements_size; i++)
		{
			ScreenElement* childElement = childElements.get(i);
			if(childElement->hasLayoutRules())
			{
				childElement->autoLayoutFrame();
			}
		}
	}
	
	RectangleD ScreenElement::getFrame() const
	{
		return frame;
	}
	
	RectangleD ScreenElement::getBorderPaddedFrame() const
	{
		Vector2d center = getCenter();
		double width = frame.width-(double)(borderWidth*2);
		if(width < 0)
		{
			width = 0;
		}
		double height = frame.height-(double)(borderWidth*2);
		if(height < 0)
		{
			height = 0;
		}
		return RectangleD(center.x-(width/2), center.y-(height/2), width, height);
	}
	
	void ScreenElement::setCenter(const Vector2d& center)
	{
		RectangleD frame = getFrame();
		frame.x = center.x-(frame.width/2);
		frame.y = center.y-(frame.height/2);
		setFrame(frame);
	}
	
	Vector2d ScreenElement::getCenter() const
	{
		RectangleD frame = getFrame();
		return Vector2d(frame.x+(frame.width/2), frame.y+(frame.height/2));
	}
	
	void ScreenElement::addChildElement(ScreenElement*element)
	{
		if(element == nullptr)
		{
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != nullptr)
		{
			throw IllegalArgumentException("element", "already added to another ScreenElement");
		}
		childElements.add(element);
		element->parentElement = this;
		if(element->hasLayoutRules())
		{
			element->autoLayoutFrame();
		}
	}
	
	void ScreenElement::addChildElement(size_t index, fgl::ScreenElement* element)
	{
		if(element == nullptr)
		{
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != nullptr)
		{
			throw IllegalArgumentException("element", "already added to another ScreenElement");
		}
		childElements.add(index, element);
		element->parentElement = this;
		if(element->hasLayoutRules())
		{
			element->autoLayoutFrame();
		}
	}
	
	void ScreenElement::removeFromParentElement()
	{
		if(parentElement != nullptr)
		{
			size_t index = parentElement->childElements.indexOf(this);
			if(index == ArrayList<ScreenElement*>::NOT_FOUND)
			{
				throw IllegalStateException("Child ScreenElement not found in parent ScreenElement");
			}
			else
			{
				parentElement->childElements.remove(index);
				parentElement = nullptr;
			}
		}
	}
	
	void ScreenElement::bringChildElementToFront(ScreenElement* element)
	{
		if(element == nullptr)
		{
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != this)
		{
			throw IllegalArgumentException("element", "not a child of the calling element");
		}

		size_t index = childElements.indexOf(element);
		if(index == ArrayList<ScreenElement*>::NOT_FOUND)
		{
			throw IllegalStateException("Child ScreenElement not found in parent ScreenElement");
		}
		else
		{
			ScreenElement* element = childElements.get(index);
			childElements.remove(index);
			childElements.add(element);
		}
	}
	
	void ScreenElement::sendChildElementToBack(ScreenElement* element)
	{
		if(element == nullptr)
		{
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != this)
		{
			throw IllegalArgumentException("element", "not a child of the calling element");
		}
		
		size_t index = childElements.indexOf(element);
		if(index == ArrayList<ScreenElement*>::NOT_FOUND)
		{
			throw IllegalStateException("Child ScreenElement not found in parent ScreenElement");
		}
		else
		{
			ScreenElement* element = childElements.get(index);
			childElements.remove(index);
			childElements.add(0, element);
		}
	}
	
	const ArrayList<ScreenElement*>& ScreenElement::getChildElements() const
	{
		return childElements;
	}
	
	ScreenElement* ScreenElement::getParentElement() const
	{
		return parentElement;
	}
	
	void ScreenElement::setLayoutRule(const LayoutRuleType& ruleType, double value, const LayoutValueType& valueType)
	{
		autoLayoutMgr.setRule(ruleType, value, valueType);
		autoLayoutFrame();
	}
	
	void ScreenElement::setLayoutRules(const Dictionary& rules)
	{
		autoLayoutMgr.setRules(rules);
		autoLayoutFrame();
	}
	
	bool ScreenElement::hasLayoutRules() const
	{
		return autoLayoutMgr.hasRules();
	}
	
	void ScreenElement::removeAllLayoutRules()
	{
		autoLayoutMgr.removeAllRules();
	}
	
	const AutoLayoutManager& ScreenElement::getAutoLayoutManager() const
	{
		return autoLayoutMgr;
	}
	
	void ScreenElement::setBackgroundColor(const Color& color)
	{
		backgroundColor = color;
	}
	
	const Color& ScreenElement::getBackgroundColor() const
	{
		return backgroundColor;
	}

	void ScreenElement::setBorderWidth(float borderWidth_arg)
	{
		borderWidth = borderWidth_arg;
	}

	float ScreenElement::getBorderWidth() const
	{
		return borderWidth;
	}

	void ScreenElement::setBorderColor(const Color& borderColor_arg)
	{
		borderColor = borderColor_arg;
	}

	const Color& ScreenElement::getBorderColor() const
	{
		return borderColor;
	}

	void ScreenElement::setAlpha(float alpha_arg)
	{
		alpha = alpha_arg;
	}

	float ScreenElement::getAlpha() const
	{
		return alpha;
	}
	
	void ScreenElement::setVisible(bool toggle)
	{
		visible = toggle;
	}
	
	bool ScreenElement::isVisible() const
	{
		return visible;
	}
	
	bool ScreenElement::isVisibleInHeirarchy() const
	{
		if(visible)
		{
			if(parentElement!=nullptr)
			{
				return parentElement->isVisibleInHeirarchy();
			}
			return true;
		}
		return false;
	}
	
	void ScreenElement::setClippedToFrame(bool toggle)
	{
		clipsToFrame = toggle;
	}
	
	bool ScreenElement::isClippedToFrame() const
	{
		return clipsToFrame;
	}

	bool ScreenElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		//Open for implementation
		return false;
	}

	void ScreenElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		//Open for implementation
	}

	bool ScreenElement::sendTouchEvent(const TouchEvent& touchEvent)
	{
		if(handleTouchEvent(touchEvent))
		{
			ArrayList<ScreenElement*> elements = childElements;
			for(size_t i=(elements.size()-1); i!=-1; i--)
			{
				ScreenElement* element = elements[i];
				RectangleD frame = element->getFrame();
				element->sendHandledTouchEvent(touchEvent.relativeTo(Vector2d(frame.x, frame.y)));
			}
			return true;
		}
		else
		{
			bool handled = false;
			ArrayList<ScreenElement*> elements = childElements;
			for(size_t i=(elements.size()-1); i!=-1; i--)
			{
				ScreenElement* element = elements[i];
				RectangleD frame = element->getFrame();
				if(handled)
				{
					element->sendHandledTouchEvent(touchEvent.relativeTo(Vector2d(frame.x, frame.y)));
				}
				else
				{
					if(element->sendTouchEvent(touchEvent.relativeTo(Vector2d(frame.x, frame.y))))
					{
						handled = true;
					}
				}
			}
			return handled;
		}
	}

	void ScreenElement::sendHandledTouchEvent(const TouchEvent& touchEvent)
	{
		otherElementHandledTouchEvent(touchEvent);
		ArrayList<ScreenElement*> elements = childElements;
		for(size_t i=(elements.size()-1); i!=-1; i--)
		{
			ScreenElement* element = elements[i];
			RectangleD frame = element->getFrame();
			element->sendHandledTouchEvent(touchEvent.relativeTo(Vector2d(frame.x, frame.y)));
		}
	}

	ScreenElement::TouchEvent::TouchEvent(const EventType& eventType, unsigned int touchID, ApplicationData appData, const Vector2d& position, bool isMouse)
		: eventType(eventType),
		touchID(touchID),
		appData(appData),
		position(position),
		mouse(isMouse)
	{
		//
	}

	ScreenElement::TouchEvent ScreenElement::TouchEvent::relativeTo(const Vector2d& point) const
	{
		ApplicationData newAppData = appData;
		newAppData.getTransform().translate(point);
		return TouchEvent(eventType, touchID, newAppData, position-point, mouse);
	}

	const ScreenElement::TouchEvent::EventType& ScreenElement::TouchEvent::getEventType() const
	{
		return eventType;
	}

	unsigned int ScreenElement::TouchEvent::getTouchID() const
	{
		return touchID;
	}

	const ApplicationData& ScreenElement::TouchEvent::getApplicationData() const
	{
		return appData;
	}

	const Vector2d& ScreenElement::TouchEvent::getPosition() const
	{
		return position;
	}

	bool ScreenElement::TouchEvent::isMouseEvent() const
	{
		return mouse;
	}
}
