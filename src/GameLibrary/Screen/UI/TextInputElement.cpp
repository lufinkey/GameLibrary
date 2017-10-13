
#include <GameLibrary/Screen/UI/TextInputElement.hpp>
#include <GameLibrary/Input/Keyboard.hpp>
#include <GameLibrary/Input/Mouse.hpp>
#include <GameLibrary/Input/Multitouch.hpp>
#include <mutex>

namespace fgl
{
	TextInputElement* TextInputElement_input_responder = nullptr;
	
	TextInputElement::TextInputListener::TextInputListener(TextInputElement* element) : element(element)
	{
		//
	}
	
	void TextInputElement::TextInputListener::onTextInput(const String& text)
	{
		element->handleTextInput(text);
	}
	
	void TextInputElement::TextInputListener::onKeyPress(Keyboard::Key key)
	{
		if(key==Keyboard::BACKSPACE)
		{
			element->handleBackspace();
		}
	}

	TextInputElement::TextInputElement() : TextInputElement(RectangleD(0, 0, 0, 0))
	{
		//
	}
	
	TextInputElement::TextInputElement(const RectangleD& frame)
		: TouchElement(frame),
		textElement(new TextElement()),
		cursorIndex(0),
		editable(true),
		resignsOnOutsideTouch(false),
		textInputListener(this)
	{
		setBackgroundColor(Colors::WHITE);
		setBorderWidth(1.0f);
		setBorderColor(Colors::BLACK);
		
		textElement->setTextAlignment(TEXTALIGN_LEFT);
		textElement->setVerticalTextAlignment(VERTICALALIGN_CENTER);
		textElement->setLayoutRule(LAYOUTRULE_TOP, 0);
		textElement->setLayoutRule(LAYOUTRULE_LEFT, 5);
		textElement->setLayoutRule(LAYOUTRULE_RIGHT, 0);
		textElement->setLayoutRule(LAYOUTRULE_BOTTOM, 0);
		
		addChildElement(textElement);
	}
	
	TextInputElement::~TextInputElement()
	{
		if(TextInputElement_input_responder==this)
		{
			resignTextInputResponder();
		}
		delete textElement;
	}
	
	void TextInputElement::update(ApplicationData appData)
	{
		TouchElement::update(appData);
	}
	
	bool TextInputElement::becomeTextInputResponder()
	{
		if(!editable)
		{
			return false;
		}
		if(TextInputElement_input_responder != nullptr)
		{
			if(TextInputElement_input_responder == this)
			{
				return true;
			}
			TextInputElement_input_responder->resignTextInputResponder();
			TextInputElement_input_responder = nullptr;
		}
		TextInputElement_input_responder = this;
		Keyboard::addEventListener(&textInputListener);
		Keyboard::startTextInput();
		return true;
	}
	
	void TextInputElement::resignTextInputResponder()
	{
		if(TextInputElement_input_responder == nullptr)
		{
			return;
		}
		if(TextInputElement_input_responder != this)
		{
			return;
		}
		TextInputElement_input_responder = nullptr;
		Keyboard::removeEventListener(&textInputListener);
		Keyboard::endTextInput();
	}
	
	bool TextInputElement::isTextInputResponder() const
	{
		if(TextInputElement_input_responder == this)
		{
			return true;
		}
		return false;
	}
	
	void TextInputElement::handleTextInput(const String& inputted_text)
	{
		if(TextInputElement_input_responder != this)
		{
			return;
		}
		String text = textElement->getText();
		text += inputted_text;
		cursorIndex += inputted_text.length();
		textElement->setText(text);
	}
	
	void TextInputElement::handleBackspace()
	{
		if(TextInputElement_input_responder != this)
		{
			return;
		}
		if(cursorIndex!=0)
		{
			String text = textElement->getText();
			text = text.substring(0,cursorIndex-1) + text.substring(cursorIndex,text.length());
			cursorIndex -= 1;
			textElement->setText(text);
		}
	}
	
	void TextInputElement::setText(const String& text)
	{
		textElement->setText(text);
		cursorIndex = text.length();
	}
	
	const String& TextInputElement::getText() const
	{
		return textElement->getText();
	}
	
	void TextInputElement::setFont(Font* font)
	{
		textElement->setFont(font);
	}
	
	Font* TextInputElement::getFont() const
	{
		return textElement->getFont();
	}

	void TextInputElement::setFontSize(unsigned int fontSize)
	{
		textElement->setFontSize(fontSize);
	}

	unsigned int TextInputElement::getFontSize() const
	{
		return textElement->getFontSize();
	}
	
	void TextInputElement::setCursorIndex(size_t index)
	{
		if(index > textElement->getText().length())
		{
			throw OutOfBoundsException((String)"cursorIndex is out of bounds at index " + index);
		}
		cursorIndex = index;
	}
	
	size_t TextInputElement::getCursorIndex() const
	{
		return cursorIndex;
	}

	void TextInputElement::setResigningOnOutsideTouchEnabled(bool toggle)
	{
		resignsOnOutsideTouch = toggle;
	}

	bool TextInputElement::isResigningOnOutsideTouchEnabled() const
	{
		return resignsOnOutsideTouch;
	}
	
	void TextInputElement::setEditable(bool editable_arg)
	{
		editable = editable_arg;
		if(!editable && isTextInputResponder())
		{
			resignTextInputResponder();
		}
	}
	
	bool TextInputElement::isEditable() const
	{
		return editable;
	}
	
	TextElement* TextInputElement::getTextElement() const
	{
		return textElement;
	}
	
	void TextInputElement::onTouchUpInside(const TouchEvent& evt)
	{
		TouchElement::onTouchUpInside(evt);
		if(!isTextInputResponder() && editable)
		{
			becomeTextInputResponder();
		}
	}

	bool TextInputElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		bool retVal = TouchElement::handleTouchEvent(touchEvent);
		if(resignsOnOutsideTouch && !retVal && touchEvent.getEventType()==TouchEvent::EVENTTYPE_TOUCHDOWN && isTextInputResponder())
		{
			resignTextInputResponder();
		}
		return retVal;
	}

	void TextInputElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		TouchElement::otherElementHandledTouchEvent(touchEvent);
		if(resignsOnOutsideTouch && touchEvent.getEventType()==TouchEvent::EVENTTYPE_TOUCHDOWN && isTextInputResponder())
		{
			resignTextInputResponder();
		}
	}
}
