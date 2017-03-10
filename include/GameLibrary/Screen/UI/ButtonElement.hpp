
#pragma once

#include <functional>
#include "TouchElement.hpp"
#include "ImageElement.hpp"
#include "TextElement.hpp"

namespace fgl
{
	class ButtonElement : public TouchElement
	{
	public:
		typedef enum
		{
			BUTTONSTATE_NORMAL,
			BUTTONSTATE_HOVERED,
			BUTTONSTATE_PRESSED
		} ButtonState;
		
		ButtonElement();
		ButtonElement(const RectangleD& frame);
		ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& handler);
		virtual ~ButtonElement();
		
		virtual void layoutChildElements() override;
		
		void setHandler(const std::function<void()>& handler);
		const std::function<void()>& getHandler() const;
		
		ButtonState getButtonState() const;
		
		void setTitle(const String& title, ButtonState state);
		const String& getTitle(ButtonState state) const;
		
		void setTitleColor(const Color& titleColor, ButtonState state);
		const Color& getTitleColor(ButtonState state) const;
		
		void setImage(TextureImage* image, ButtonState state);
		TextureImage* getImage(ButtonState state) const;
		
		TextElement* getTitleElement() const;
		ImageElement* getImageElement() const;
		
	protected:
		virtual void onMouseEnter(unsigned int mouseIndex) override;
		virtual void onMouseLeave(unsigned int mouseIndex) override;
		
		virtual void onTouchDown(const TouchEvent& touchEvent) override;
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		virtual void onTouchUpOutside(const TouchEvent& touchEvent) override;
		virtual void onTouchCancel(const TouchEvent& touchEvent) override;
		
	private:
		void setButtonState(ButtonState buttonState);
		
		std::function<void()> handler;
		
		ImageElement* imageElement;
		TextElement* titleElement;
		
		ButtonState buttonState;
		BasicDictionary<ButtonState, String> titles;
		BasicDictionary<ButtonState, Color> titleColors;
		BasicDictionary<ButtonState, TextureImage*> images;
	};
}