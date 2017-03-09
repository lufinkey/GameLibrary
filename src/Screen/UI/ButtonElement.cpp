
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
	
	ButtonElement::ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& handler)
		: TouchElement(frame),
		handler(handler),
		imageElement(new ImageElement(RectangleD(0,0,frame.width,0))),
		titleElement(new TextElement(RectangleD(0,0,frame.width,frame.height)))
	{
		titles[BUTTONSTATE_NORMAL] = title;
		titleColors[BUTTONSTATE_NORMAL] = Color::BLACK;
		images[BUTTONSTATE_NORMAL] = nullptr;
		
		titleElement->setText(title);
		titleElement->setTextColor(Color::BLACK);
		
		addChildElement(titleElement);
		addChildElement(imageElement);
	}
	
	ButtonElement::~ButtonElement()
	{
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
	
	void ButtonElement::setHandler(const std::function<void()>& handler_arg)
	{
		handler = handler_arg;
	}
	
	const std::function<void()>& ButtonElement::getHandler() const
	{
		return handler;
	}
	
	void ButtonElement::setButtonState(ButtonState buttonState_arg)
	{
		buttonState = buttonState_arg;
		imageElement->setImage(getImage(buttonState));
		titleElement->setText(getTitle(buttonState));
		titleElement->setTextColor(getTitleColor(buttonState));
		layoutChildElements();
	}
	
	ButtonElement::ButtonState ButtonElement::getButtonState() const
	{
		return buttonState;
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
			TextureImage* image = images.get(state);
			if(image==nullptr)
			{
				return images.get(BUTTONSTATE_NORMAL);
			}
			else
			{
				return image;
			}
		}
		catch(const DictionaryKeyNotFoundException&)
		{
			return images.get(BUTTONSTATE_NORMAL);
		}
	}
	
	void ButtonElement::onMouseEnter(unsigned int mouseIndex)
	{
		//TODO should do something here related to BUTTONSTATE_HOVERED
	}
	
	void ButtonElement::onMouseLeave(unsigned int mouseIndex)
	{
		//TODO should do something here related to BUTTONSTATE_HOVERED
	}
	
	void ButtonElement::onTouchDown(const TouchEvent& touchEvent)
	{
		setButtonState(BUTTONSTATE_PRESSED);
	}
	
	void ButtonElement::onTouchUpInside(const TouchEvent& touchEvent)
	{
		//TODO do BUTTONSTATE_HOVERED if still hovered
		setButtonState(BUTTONSTATE_NORMAL);
		if(handler)
		{
			handler();
		}
	}
	
	void ButtonElement::onTouchUpOutside(const TouchEvent& touchEvent)
	{
		setButtonState(BUTTONSTATE_NORMAL);
	}
	
	void ButtonElement::onTouchCancel(const TouchEvent& touchEvent)
	{
		setButtonState(BUTTONSTATE_NORMAL);
	}
}
