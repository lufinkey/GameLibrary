
#pragma once

#include <GameLibrary/Input/Keyboard.hpp>
#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Utilities/Font/Font.hpp>
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
		/*! virtual destructor*/
		virtual ~TextInputElement();
		
		/*! \copydoc fgl::TouchElement::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		/*! \copydoc fgl::TouchElement::drawMain(fgl::ApplicationData)const*/
		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;
		
		virtual void becomeTextInputResponder();
		virtual void resignTextInputResponder();
		bool isTextInputResponder() const;
		
		void setText(const String& text);
		const String& getText() const;
		
		void setFont(Font* font);
		Font* getFont() const;
		void setFontSize(unsigned int fontSize);
		unsigned int getFontSize() const;
		
		void setCursorIndex(size_t cursorIndex);
		size_t getCursorIndex() const;
		
	protected:
		virtual void onTouchUpInside(const TouchEvent& evt) override;
		
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

		String text;
		Color textColor;
		Font* font;
		unsigned int fontSize;
		size_t cursorIndex;
		
		TextInputListener textInputListener;
	};
}
