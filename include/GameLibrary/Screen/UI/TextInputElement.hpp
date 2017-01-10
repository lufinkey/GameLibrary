
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
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit TextInputElement(const RectangleD&frame);
		/*! virtual destructor*/
		virtual ~TextInputElement();
		
		/*! \copydoc fgl::ScreenElement::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		/*! \copydoc fgl::ScreenElement::drawMain(fgl::ApplicationData)const*/
		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;
		
		virtual void becomeTextInputResponder();
		virtual void resignTextInputResponder();
		bool isTextInputResponder() const;
		
		void setText(const String& text);
		const String& getText() const;
		
		void setFont(Font* font);
		Font* getFont() const;
		
		void setBorderColor(const Color& borderColor);
		const Color& getBorderColor() const;
		
		void setCursorIndex(size_t cursorIndex);
		size_t getCursorIndex() const;
		
		virtual void onTouchUpInside(const TouchElementEvent& evt) override;
		
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
		size_t cursorIndex;
		Color borderColor;
		
		TextInputListener textInputListener;
		
		virtual ScreenElement* handleMouseRelease(const ApplicationData& appData, unsigned int mouseIndex, Mouse::Button button, const Vector2d& mousepos) override;
		virtual void elementHandledMouseRelease(const ApplicationData& appData, unsigned int mouseIndex, Mouse::Button button, const Vector2d& mousepos, ScreenElement* element) override;
		
		virtual ScreenElement* handleTouchEnd(const ApplicationData& appData, unsigned int touchID, const Vector2d& touchpos) override;
		virtual void elementHandledTouchEnd(const ApplicationData& appData, unsigned int touchID, const Vector2d& touchpos, ScreenElement* element) override;
	};
}
