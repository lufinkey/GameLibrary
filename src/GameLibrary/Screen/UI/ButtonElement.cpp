
#include <GameLibrary/Screen/UI/ButtonElement.hpp>

namespace fgl
{
	ButtonElement::ButtonElement() : ButtonElement(RectangleD(0,0,100,60))
	{
		//
	}
	
	ButtonElement::ButtonElement(const RectangleD& frame) : ButtonElement(frame, "", nullptr)
	{
		//
	}
	
	ButtonElement::ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& tapHandler)
		: TouchElement(frame),
		tapHandler(tapHandler),
		backgroundElement(new ImageElement(RectangleD(0,0, frame.width, frame.height))),
		imageElement(new ImageElement(RectangleD(0,0,frame.width,0))),
		titleElement(new TextElement(RectangleD(0,0,frame.width,frame.height))),
		buttonState(BUTTONSTATE_NORMAL),
		enabled(true),
		selected(false),
		pressed(false),
		pressedTouchID(-1)
	{
		titles[BUTTONSTATE_NORMAL] = title;
		titleColors[BUTTONSTATE_NORMAL] = Color::BLACK;
		images[BUTTONSTATE_NORMAL] = nullptr;
		backgroundImages[BUTTONSTATE_NORMAL] = nullptr;
		backgroundColors[BUTTONSTATE_NORMAL] = Color::TRANSPARENT;
		tintColors[BUTTONSTATE_NORMAL] = Color::WHITE;
		
		backgroundElement->setLayoutRule(LAYOUTRULE_LEFT, 0);
		backgroundElement->setLayoutRule(LAYOUTRULE_TOP, 0);
		backgroundElement->setLayoutRule(LAYOUTRULE_RIGHT, 0);
		backgroundElement->setLayoutRule(LAYOUTRULE_BOTTOM, 0);

		titleElement->setText(title);
		titleElement->setTextColor(Color::BLACK);
		titleElement->setTextAlignment(TEXTALIGN_CENTER);
		titleElement->setVerticalTextAlignment(VERTICALALIGN_CENTER);
		
		addChildElement(backgroundElement);
		addChildElement(titleElement);
		addChildElement(imageElement);
	}
	
	ButtonElement::~ButtonElement()
	{
		delete backgroundElement;
		delete imageElement;
		delete titleElement;
	}
	
	void ButtonElement::layoutChildElements()
	{
		TouchElement::layoutChildElements();
		RectangleD frame = getFrame();
		if(imageElement->getImage()!=nullptr)
		{
			if(titleElement->getText().length() > 0)
			{
				double textHeight = titleElement->measureTextHeight(frame.width);
				if(textHeight > (frame.height/2))
				{
					textHeight = frame.height/2;
				}
				double imageHeight = frame.height - textHeight;
				if(imageHeight < 0)
				{
					imageHeight = 0;
				}
				imageElement->setFrame(RectangleD(0,0,frame.width,imageHeight));
				titleElement->setFrame(RectangleD(0,imageHeight,frame.width,textHeight));
			}
			else
			{
				imageElement->setFrame(RectangleD(0,0,frame.width,frame.height));
				titleElement->setFrame(RectangleD(0,frame.height,frame.width,0));
			}
		}
		else
		{
			imageElement->setFrame(RectangleD(0,0,frame.width,0));
			titleElement->setFrame(RectangleD(0,0,frame.width,frame.height));
		}
	}
	
	void ButtonElement::draw(ApplicationData appData, Graphics graphics) const
	{
		auto tintColor = getTintColor(buttonState);
		if(tintColor!=Color::WHITE)
		{
			graphics.compositeTintColor(tintColor);
		}
		TouchElement::draw(appData, graphics);
	}
	
	void ButtonElement::setTapHandler(const std::function<void()>& tapHandler_arg)
	{
		tapHandler = tapHandler_arg;
	}
	
	const std::function<void()>& ButtonElement::getTapHandler() const
	{
		return tapHandler;
	}
	
	void ButtonElement::setButtonState(ButtonState buttonState_arg)
	{
		if(buttonState != buttonState_arg)
		{
			buttonState = buttonState_arg;
			updateStateProperties();
			layoutChildElements();
		}
	}
	
	void ButtonElement::updateButtonState()
	{
		if(!enabled)
		{
			setButtonState(BUTTONSTATE_DISABLED);
		}
		else if(pressed)
		{
			setButtonState(BUTTONSTATE_PRESSED);
		}
		else if(selected)
		{
			setButtonState(BUTTONSTATE_SELECTED);
		}
		else if(getHoveredMouseIndexes().size() > 0)
		{
			setButtonState(BUTTONSTATE_HOVERED);
		}
		else
		{
			setButtonState(BUTTONSTATE_NORMAL);
		}
	}

	void ButtonElement::updateStateProperties()
	{
		imageElement->setImage(getImage(buttonState));
		titleElement->setText(getTitle(buttonState));
		titleElement->setTextColor(getTitleColor(buttonState));
		backgroundElement->setImage(getBackgroundImage(buttonState));
		backgroundElement->setBackgroundColor(getBackgroundColor(buttonState));
	}
	
	ButtonElement::ButtonState ButtonElement::getButtonState() const
	{
		return buttonState;
	}

	void ButtonElement::setEnabled(bool enabled_arg)
	{
		enabled = enabled_arg;
		if(!enabled)
		{
			pressed = false;
			pressedTouchID = -1;
		}
		updateButtonState();
	}

	bool ButtonElement::isEnabled() const
	{
		return enabled;
	}
	
	void ButtonElement::setSelected(bool selected_arg)
	{
		selected = selected_arg;
		updateButtonState();
	}
	
	void ButtonElement::setTitle(const String& title, ButtonState state)
	{
		titles[state] = title;
		updateStateProperties();
	}
	
	void ButtonElement::setTitle(std::nullptr_t, ButtonState state)
	{
		titles.remove(state);
		updateStateProperties();
	}
	
	String ButtonElement::getTitle(ButtonState state) const
	{
		return getProperty<String>(titles, state, "");
	}
	
	void ButtonElement::setTitleColor(const fgl::Color& titleColor, ButtonState state)
	{
		titleColors[state] = titleColor;
		updateStateProperties();
	}
	
	void ButtonElement::setTitleColor(std::nullptr_t, ButtonState state)
	{
		titleColors.remove(state);
		updateStateProperties();
	}
	
	Color ButtonElement::getTitleColor(ButtonState state) const
	{
		return getProperty<Color>(titleColors, state, Color::TRANSPARENT);
	}
	
	void ButtonElement::setTintColor(const fgl::Color& tintColor, ButtonState state)
	{
		tintColors[state] = tintColor;
		updateStateProperties();
	}
	
	void ButtonElement::setTintColor(std::nullptr_t, ButtonState state)
	{
		tintColors.remove(state);
		updateStateProperties();
	}
	
	Color ButtonElement::getTintColor(ButtonState state) const
	{
		return getProperty<Color>(tintColors, state, Color::TRANSPARENT);
	}
	
	void ButtonElement::setImage(TextureImage* image, ButtonState state)
	{
		if(image==nullptr)
		{
			images.remove(state);
		}
		else
		{
			images[state] = image;
		}
		updateStateProperties();
	}
	
	void ButtonElement::setImage(std::nullptr_t, ButtonState state)
	{
		images.remove(state);
		updateStateProperties();
	}
	
	TextureImage* ButtonElement::getImage(ButtonState state) const
	{
		return getProperty<TextureImage*>(images, state, nullptr);
	}

	void ButtonElement::setBackgroundImage(TextureImage* image, ButtonState state)
	{
		if(image==nullptr)
		{
			backgroundImages.remove(state);
		}
		else
		{
			backgroundImages[state] = image;
		}
		updateStateProperties();
	}
	
	void ButtonElement::setBackgroundImage(std::nullptr_t, ButtonState state)
	{
		backgroundImages.remove(state);
		updateStateProperties();
	}

	TextureImage* ButtonElement::getBackgroundImage(ButtonState state) const
	{
		return getProperty<TextureImage*>(backgroundImages, state, nullptr);
	}
	
	void ButtonElement::setBackgroundColor(const Color& color, ButtonState state)
	{
		backgroundColors[state] = color;
		updateStateProperties();
	}
	
	void ButtonElement::setBackgroundColor(std::nullptr_t, ButtonState state)
	{
		backgroundColors.remove(state);
		updateStateProperties();
	}
	
	Color ButtonElement::getBackgroundColor(ButtonState state) const
	{
		return getProperty<Color>(backgroundColors, state, Color::TRANSPARENT);
	}
	
	TextElement* ButtonElement::getTitleElement() const
	{
		return titleElement;
	}
	
	ImageElement* ButtonElement::getImageElement() const
	{
		return imageElement;
	}
	
	void ButtonElement::onMouseEnter(unsigned int mouseIndex)
	{
		updateButtonState();
	}
	
	void ButtonElement::onMouseLeave(unsigned int mouseIndex)
	{
		updateButtonState();
	}
	
	void ButtonElement::onTouchDown(const TouchEvent& touchEvent)
	{
		if(enabled && !pressed)
		{
			pressed = true;
			pressedTouchID = touchEvent.getTouchID();
			updateButtonState();
		}
	}
	
	void ButtonElement::onTouchUpInside(const TouchEvent& touchEvent)
	{
		if(enabled && pressed && pressedTouchID==touchEvent.getTouchID())
		{
			pressed = false;
			pressedTouchID = -1;
			updateButtonState();
			if(tapHandler)
			{
				tapHandler();
			}
		}
	}
	
	void ButtonElement::onTouchUpOutside(const TouchEvent& touchEvent)
	{
		if(enabled && pressed && pressedTouchID==touchEvent.getTouchID())
		{
			pressed = false;
			pressedTouchID = -1;
			updateButtonState();
		}
	}
	
	void ButtonElement::onTouchCancel(const TouchEvent& touchEvent)
	{
		if(enabled && pressed && pressedTouchID==touchEvent.getTouchID())
		{
			pressed = false;
			pressedTouchID = -1;
			updateButtonState();
		}
	}
}
