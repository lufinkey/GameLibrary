
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
		enabled(true)
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
		buttonState = buttonState_arg;
		imageElement->setImage(getImage(buttonState));
		titleElement->setText(getTitle(buttonState));
		titleElement->setTextColor(getTitleColor(buttonState));
		backgroundElement->setImage(getBackgroundImage(buttonState));
		backgroundElement->setBackgroundColor(getBackgroundColor(buttonState));
		layoutChildElements();
	}
	
	ButtonElement::ButtonState ButtonElement::getButtonState() const
	{
		return buttonState;
	}

	void ButtonElement::setEnabled(bool enabled_arg)
	{
		if(enabled_arg && !enabled)
		{
			enabled = true;
			setButtonState(BUTTONSTATE_NORMAL);
		}
		else if(!enabled_arg && enabled)
		{
			enabled = false;
			setButtonState(BUTTONSTATE_DISABLED);
		}
	}

	bool ButtonElement::isEnabled() const
	{
		return enabled;
	}
	
	void ButtonElement::setTitle(const String& title, ButtonState state)
	{
		titles[state] = title;
		if(state==buttonState && title!=titleElement->getText())
		{
			titleElement->setText(title);
			layoutChildElements();
		}
	}
	
	const String& ButtonElement::getTitle(ButtonState state) const
	{
		try
		{
			return titles.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return titles.get(BUTTONSTATE_NORMAL);
		}
	}
	
	void ButtonElement::setTitleColor(const fgl::Color& titleColor, ButtonState state)
	{
		titleColors[state] = titleColor;
		if(state==buttonState)
		{
			titleElement->setTextColor(titleColor);
		}
	}
	
	const Color& ButtonElement::getTitleColor(ButtonState state) const
	{
		try
		{
			return titleColors.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return titleColors.get(BUTTONSTATE_NORMAL);
		}
	}
	
	void ButtonElement::setTintColor(const fgl::Color& tintColor, ButtonState state)
	{
		tintColors[state] = tintColor;
	}
	
	const Color& ButtonElement::getTintColor(ButtonState state) const
	{
		try
		{
			return tintColors.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return tintColors.get(BUTTONSTATE_NORMAL);
		}
	}
	
	void ButtonElement::setImage(TextureImage* image, ButtonState state)
	{
		images[state] = image;
		if(state==buttonState)
		{
			imageElement->setImage(image);
			layoutChildElements();
		}
	}
	
	TextureImage* ButtonElement::getImage(ButtonState state) const
	{
		try
		{
			return images.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return images.get(BUTTONSTATE_NORMAL);
		}
	}

	void ButtonElement::setBackgroundImage(TextureImage* image, ButtonState state)
	{
		backgroundImages[state] = image;
		if(state==buttonState)
		{
			backgroundElement->setImage(image);
		}
	}

	TextureImage* ButtonElement::getBackgroundImage(ButtonState state) const
	{
		try
		{
			return backgroundImages.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return backgroundImages.get(BUTTONSTATE_NORMAL);
		}
	}
	
	void ButtonElement::setBackgroundColor(const Color& color, ButtonState state)
	{
		backgroundColors[state] = color;
		if(state==buttonState)
		{
			backgroundElement->setBackgroundColor(color);
		}
	}
	
	const Color& ButtonElement::getBackgroundColor(ButtonState state) const
	{
		try
		{
			return backgroundColors.get(state);
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return backgroundColors.get(BUTTONSTATE_NORMAL);
		}
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
		if(enabled)
		{
			if(buttonState!=BUTTONSTATE_PRESSED && getHoveredMouseIndexes().size() > 0)
			{
				setButtonState(BUTTONSTATE_HOVERED);
			}
		}
	}
	
	void ButtonElement::onMouseLeave(unsigned int mouseIndex)
	{
		if(enabled)
		{
			if(buttonState==BUTTONSTATE_HOVERED && getHoveredMouseIndexes().size()==0)
			{
				setButtonState(BUTTONSTATE_NORMAL);
			}
		}
	}
	
	void ButtonElement::onTouchDown(const TouchEvent& touchEvent)
	{
		if(enabled)
		{
			setButtonState(BUTTONSTATE_PRESSED);
		}
	}
	
	void ButtonElement::onTouchUpInside(const TouchEvent& touchEvent)
	{
		if(enabled)
		{
			if(getHoveredMouseIndexes().size() > 0)
			{
				setButtonState(BUTTONSTATE_HOVERED);
			}
			else
			{
				setButtonState(BUTTONSTATE_NORMAL);
			}
			if(tapHandler)
			{
				tapHandler();
			}
		}
	}
	
	void ButtonElement::onTouchUpOutside(const TouchEvent& touchEvent)
	{
		if(enabled)
		{
			if(getHoveredMouseIndexes().size() > 0)
			{
				setButtonState(BUTTONSTATE_HOVERED);
			}
			else
			{
				setButtonState(BUTTONSTATE_NORMAL);
			}
		}
	}
	
	void ButtonElement::onTouchCancel(const TouchEvent& touchEvent)
	{
		if(enabled)
		{
			if(getHoveredMouseIndexes().size() > 0)
			{
				setButtonState(BUTTONSTATE_HOVERED);
			}
			else
			{
				setButtonState(BUTTONSTATE_NORMAL);
			}
		}
	}
}
