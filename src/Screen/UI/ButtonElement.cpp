
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
		titleElement->setText(title);
		
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
	
	void ButtonElement::setTitle(const String& title)
	{
		titleElement->setText(title);
		layoutChildElements();
	}
	
	const String& ButtonElement::getTitle() const
	{
		return titleElement->getText();
	}
	
	void ButtonElement::setTitleColor(const fgl::Color& titleColor)
	{
		titleElement->setTextColor(titleColor);
	}
	
	const Color& ButtonElement::getTitleColor() const
	{
		return titleElement->getTextColor();
	}
	
	void ButtonElement::setImage(TextureImage* image)
	{
		imageElement->setImage(image);
		layoutChildElements();
	}
	
	TextureImage* ButtonElement::getImage() const
	{
		return imageElement->getImage();
	}
	
	void ButtonElement::onTouchUpInside(const TouchEvent& touchEvent)
	{
		if(handler)
		{
			handler();
		}
	}
}
