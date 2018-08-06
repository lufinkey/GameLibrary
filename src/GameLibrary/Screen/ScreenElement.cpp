
#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>

namespace fgl
{
	void ScreenElement::autoLayoutFrame()
	{
		if(parentElement!=nullptr) {
			setFrame(autoLayoutMgr.calculateFrame(frame, parentElement->getFrame()));
		}
	}
	
	ScreenElement::ScreenElement() : ScreenElement(RectangleD(0, 0, 0, 0))
	{
		//
	}
	
	ScreenElement::ScreenElement(const RectangleD& frame)
		: frame(frame),
		parentElement(nullptr),
		backgroundColor(Colors::TRANSPARENT),
		borderWidth(0),
		borderColor(Colors::BLACK),
		alpha(1.0),
		visible(true),
		clipsToFrame(false),
		needsLayout(true)
	{
		//
	}
	
	ScreenElement::~ScreenElement()
	{
		for(auto element : childElements) {
			delete element;
		}
	}
	
	void ScreenElement::update(ApplicationData appData) {
		layoutChildElementsIfNeeded();
		updateElements(appData);
	}
	
	void ScreenElement::onLayoutChildElements() {
		// open for implementation
	}
	
	void ScreenElement::updateElements(ApplicationData appData)
	{
		appData = getChildrenApplicationData(appData);
		ArrayList<ScreenElement*> children = childElements;
		for(size_t i=0; i<children.size(); i++)
		{
			ScreenElement* element = children.get(i);
			element->update(appData);
		}
	}
	
	void ScreenElement::drawBackground(ApplicationData appData, Graphics graphics) const
	{
		if(!backgroundColor.equals(Colors::TRANSPARENT))
		{
			RectangleD rect = getFrame();
			graphics.setColor(backgroundColor);
			graphics.fillRect(rect.x, rect.y, rect.width, rect.height);
		}
	}

	void ScreenElement::drawBorder(ApplicationData appData, Graphics graphics) const
	{
		if(borderWidth != 0 && !borderColor.equals(Colors::TRANSPARENT))
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
		appData = getChildrenApplicationData(appData);
		auto childGraphics = getChildrenGraphics(graphics);
		ArrayList<ScreenElement*> children = childElements;
		for(auto element : children)
		{
			element->draw(appData, childGraphics);
		}
	}
	
	void ScreenElement::draw(ApplicationData appData, Graphics graphics) const
	{
		if(visible)
		{
			auto frame = getFrame();
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
	
	ApplicationData ScreenElement::getChildrenApplicationData(ApplicationData appData) const
	{
		auto frame = getFrame();
		appData.getTransform().translate(frame.x, frame.y);
		return appData;
	}
	
	Graphics ScreenElement::getChildrenGraphics(Graphics graphics) const
	{
		auto frame = getFrame();
		graphics.translate(frame.x, frame.y);
		return graphics;
	}
	
	void ScreenElement::setFrame(const RectangleD& frame_arg)
	{
		frame = frame_arg;
		setNeedsLayout();
	}
	
	void ScreenElement::layoutChildElements() {
		needsLayout = false;
		for(size_t childElements_size=childElements.size(), i=0; i<childElements_size; i++) {
			ScreenElement* childElement = childElements[i];
			if(childElement->hasLayoutRules()) {
				childElement->autoLayoutFrame();
			}
		}
		onLayoutChildElements();
	}
	
	void ScreenElement::layoutChildElementsIfNeeded() {
		if(needsLayout) {
			layoutChildElements();
		}
	}
	
	void ScreenElement::setNeedsLayout() {
		needsLayout = true;
	}
	
	RectangleD ScreenElement::getFrame() const {
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
	
	void ScreenElement::addChildElement(ScreenElement*element) {
		if(element == nullptr) {
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != nullptr) {
			throw IllegalArgumentException("element", "already added to another ScreenElement");
		}
		childElements.add(element);
		element->parentElement = this;
		setNeedsLayout();
		element->onAddToScreenElement(this);
	}
	
	void ScreenElement::addChildElement(size_t index, fgl::ScreenElement* element) {
		if(element == nullptr) {
			throw IllegalArgumentException("element", "cannot be null");
		}
		else if(element->parentElement != nullptr) {
			throw IllegalArgumentException("element", "already added to another ScreenElement");
		}
		childElements.add(index, element);
		element->parentElement = this;
		setNeedsLayout();
		element->onAddToScreenElement(this);
	}
	
	void ScreenElement::removeFromParentElement()
	{
		if(parentElement == nullptr) {
			return;
		}
		size_t index = parentElement->childElements.indexOf(this);
		if(index == ArrayList<ScreenElement*>::NOT_FOUND) {
			throw IllegalStateException("Child ScreenElement not found in parent ScreenElement");
		}
		auto oldParentElement = parentElement;
		parentElement->childElements.remove(index);
		parentElement = nullptr;
		onRemoveFromScreenElement(oldParentElement);
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
			auto childEvent = touchEvent.withAppData(getChildrenApplicationData(touchEvent.getApplicationData()));
			ArrayList<ScreenElement*> elements = childElements;
			for(size_t i=(elements.size()-1); i!=-1; i--)
			{
				ScreenElement* element = elements[i];
				element->sendHandledTouchEvent(childEvent);
			}
			return true;
		}
		else
		{
			auto childEvent = touchEvent.withAppData(getChildrenApplicationData(touchEvent.getApplicationData()));
			bool handled = false;
			ArrayList<ScreenElement*> elements = childElements;
			for(size_t i=(elements.size()-1); i!=-1; i--)
			{
				ScreenElement* element = elements[i];
				if(handled)
				{
					element->sendHandledTouchEvent(childEvent);
				}
				else
				{
					if(element->sendTouchEvent(childEvent))
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
		auto childEvent = touchEvent.withAppData(getChildrenApplicationData(touchEvent.getApplicationData()));
		ArrayList<ScreenElement*> elements = childElements;
		for(size_t i=(elements.size()-1); i!=-1; i--)
		{
			ScreenElement* element = elements[i];
			element->sendHandledTouchEvent(childEvent);
		}
	}
	
	fgl::String ScreenElement::TouchEvent::EventType_toString(EventType eventType)
	{
		switch(eventType)
		{
			case EVENTTYPE_TOUCHDOWN:
				return "EVENTTYPE_TOUCHDOWN";
				
			case EVENTTYPE_TOUCHMOVE:
				return "EVENTTYPE_TOUCHMOVE";
				
			case EVENTTYPE_TOUCHUP:
				return "EVENTTYPE_TOUCHUP";
				
			case EVENTTYPE_TOUCHCANCEL:
				return "EVENTTYPE_TOUCHCANCEL";
		}
	}

	ScreenElement::TouchEvent::TouchEvent(const EventType& eventType, unsigned int touchID, ApplicationData appData, const Vector2d& realPosition, bool isMouse)
		: eventType(eventType),
		touchID(touchID),
		appData(appData),
		realPosition(realPosition),
		inverseTransform(appData.getTransform().getInverse()),
		mouse(isMouse)
	{
		//
	}

	ScreenElement::TouchEvent ScreenElement::TouchEvent::withAppData(const ApplicationData& newAppData) const
	{
		return TouchEvent(eventType, touchID, newAppData, realPosition, mouse);
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

	Vector2d ScreenElement::TouchEvent::getPosition() const
	{
		return inverseTransform.transform(realPosition);
	}

	bool ScreenElement::TouchEvent::isMouseEvent() const
	{
		return mouse;
	}
	
	fgl::String ScreenElement::TouchEvent::toString() const
	{
		return "ScreenElement::TouchEvent(eventType: "+EventType_toString(eventType)+", touchID: "+touchID+", position: "+getPosition().toString()+", mouse: "+mouse+")";
	}
	
	void ScreenElement::handleAddToWindow(Window* window) {
		onAddToWindow(window);
		for(auto element : childElements) {
			element->handleAddToWindow(window);
		}
	}
	
	void ScreenElement::handleRemoveFromWindow(Window* window) {
		onRemoveFromWindow(window);
		for(auto element : childElements) {
			element->handleRemoveFromWindow(window);
		}
	}
	
	void ScreenElement::onAddToWindow(Window* window) {
		// open for implementation
	}
	
	void ScreenElement::onRemoveFromWindow(Window* window) {
		// open for implementation
	}
	
	void ScreenElement::onAddToScreenElement(ScreenElement* parent) {
		// open for implementation
	}
	
	void ScreenElement::onRemoveFromScreenElement(ScreenElement* parent) {
		// open for implementation
	}
}
