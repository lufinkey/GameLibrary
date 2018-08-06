
#pragma once

#include <GameLibrary/Input/Keyboard.hpp>
#include "TextElement.hpp"
#include "TouchElement.hpp"

namespace fgl
{
	class TextInputElement : public TouchElement
	{
	public:
		/*! \copydoc fgl::ScreenElement::ScreenElement()*/
		TextInputElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit TextInputElement(const RectangleD&frame);
		
		/*! \copydoc fgl::TouchElement::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		
		virtual bool becomeTextInputResponder();
		virtual void resignTextInputResponder();
		bool isTextInputResponder() const;
		void setResigningOnOutsideTouchEnabled(bool toggle);
		bool isResigningOnOutsideTouchEnabled() const;
		
		void setText(const String& text);
		const String& getText() const;
		
		void setFont(Font* font);
		Font* getFont() const;
		void setFontSize(unsigned int fontSize);
		unsigned int getFontSize() const;
		
		void setCursorIndex(size_t cursorIndex);
		size_t getCursorIndex() const;
		
		void setEditable(bool editable);
		bool isEditable() const;
		
		TextElement* getTextElement() const;
		
	protected:
		virtual void onTouchUpInside(const TouchEvent& evt) override;

		virtual bool handleTouchEvent(const TouchEvent& touchEvent) override;
		virtual void otherElementHandledTouchEvent(const TouchEvent& touchEvent) override;
		
		virtual void onRemoveFromScreenElement(ScreenElement* parent) override;
		
	private:
		class TextInputListener : public KeyboardEventListener
		{
		private:
			TextInputElement* element;
		public:
			TextInputListener(TextInputElement* element);
			virtual void onKeyPress(Keyboard::Key key) override;
			virtual void onTextInput(const String& text) override;
		};
		
		void handleTextInput(const String& inputted_text);
		void handleBackspace();

		TextElement* textElement;
		size_t cursorIndex;
		bool editable;
		bool resignsOnOutsideTouch;
		
		TextInputListener textInputListener;
	};
}
