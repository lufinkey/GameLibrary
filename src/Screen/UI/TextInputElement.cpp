
#include <GameLibrary/Screen/UI/TextInputElement.hpp>
#include <GameLibrary/Input/Keyboard.hpp>
#include <GameLibrary/Input/Mouse.hpp>
#include <GameLibrary/Input/Multitouch.hpp>
#include <mutex>

namespace fgl
{
	TextInputElement* TextInputElement_input_responder = nullptr;
	std::mutex TextInputElement_responder_mutex;
	
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
		textColor(Color::BLACK),
		font(Graphics::getDefaultFont()),
		fontSize(24),
		cursorIndex(0),
		resignsOnOutsideTouch(false),
		textInputListener(this)
	{
		setBackgroundColor(Color::WHITE);
		setBorderWidth(1.0f);
		setBorderColor(Color::BLACK);
	}
	
	TextInputElement::~TextInputElement()
	{
		if(TextInputElement_input_responder==this)
		{
			resignTextInputResponder();
		}
	}
	
	void TextInputElement::update(ApplicationData appData)
	{
		TouchElement::update(appData);
	}
	
	void TextInputElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		TouchElement::drawMain(appData, graphics);
		RectangleD frame = getFrame();
		if(font!=nullptr)
		{
			font->setSize(fontSize);
			Vector2u textSize = font->measureString(text);
			double textY = frame.y + ((frame.height + (double)textSize.y)/2);
			double textX = frame.x + 5;
			graphics.clip(frame);
			graphics.setColor(textColor);
			graphics.setFont(font);
			graphics.drawString(text, textX, textY);
		}
	}
	
	void TextInputElement::becomeTextInputResponder()
	{
		if(TextInputElement_input_responder != nullptr)
		{
			if(TextInputElement_input_responder == this)
			{
				return;
			}
			TextInputElement_input_responder->resignTextInputResponder();
		}
		TextInputElement_responder_mutex.lock();
		TextInputElement_input_responder = this;
		Keyboard::addEventListener(&textInputListener);
		Keyboard::startTextInput();
		TextInputElement_responder_mutex.unlock();
	}
	
	void TextInputElement::resignTextInputResponder()
	{
		TextInputElement_responder_mutex.lock();
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
		TextInputElement_responder_mutex.unlock();
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
		text += inputted_text;
		cursorIndex += inputted_text.length();
	}
	
	void TextInputElement::handleBackspace()
	{
		if(TextInputElement_input_responder != this)
		{
			return;
		}
		if(cursorIndex!=0)
		{
			text = text.substring(0,cursorIndex-1) + text.substring(cursorIndex,text.length());
			cursorIndex -= 1;
		}
	}
	
	void TextInputElement::setText(const String& txt)
	{
		text = txt;
		cursorIndex = text.length();
	}
	
	const String& TextInputElement::getText() const
	{
		return text;
	}
	
	void TextInputElement::setFont(Font* fnt)
	{
		font = fnt;
	}
	
	Font* TextInputElement::getFont() const
	{
		return font;
	}

	void TextInputElement::setFontSize(unsigned int fontSize_arg)
	{
		fontSize = fontSize_arg;
	}

	unsigned int TextInputElement::getFontSize() const
	{
		return fontSize;
	}
	
	void TextInputElement::setCursorIndex(size_t index)
	{
		if(index > text.length())
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
	
	void TextInputElement::onTouchUpInside(const TouchEvent& evt)
	{
		TouchElement::onTouchUpInside(evt);
		if(!isTextInputResponder())
		{
			becomeTextInputResponder();
		}
	}

	bool TextInputElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		bool retVal = TouchElement::handleTouchEvent(touchEvent);
		if(resignsOnOutsideTouch && !retVal && isTextInputResponder())
		{
			resignTextInputResponder();
		}
		return retVal;
	}

	void TextInputElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		TouchElement::otherElementHandledTouchEvent(touchEvent);
		if(resignsOnOutsideTouch && isTextInputResponder())
		{
			resignTextInputResponder();
		}
	}
}
