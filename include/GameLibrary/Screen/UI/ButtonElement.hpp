
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
			BUTTONSTATE_PRESSED,
			BUTTONSTATE_DISABLED
		} ButtonState;
		
		ButtonElement();
		ButtonElement(const RectangleD& frame);
		ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& tapHandler);
		virtual ~ButtonElement();
		
		virtual void layoutChildElements() override;
		virtual void draw(ApplicationData appData, Graphics graphics) const override;
		
		void setTapHandler(const std::function<void()>& tapHandler);
		const std::function<void()>& getTapHandler() const;
		
		ButtonState getButtonState() const;

		void setEnabled(bool enabled);
		bool isEnabled() const;
		
		void setTitle(const String& title, ButtonState state);
		const String& getTitle(ButtonState state) const;
		
		void setTitleColor(const Color& titleColor, ButtonState state);
		const Color& getTitleColor(ButtonState state) const;
		
		void setTintColor(const Color& tintColor, ButtonState state);
		const Color& getTintColor(ButtonState state) const;
		
		void setImage(TextureImage* image, ButtonState state);
		TextureImage* getImage(ButtonState state) const;

		void setBackgroundImage(TextureImage* image, ButtonState state);
		TextureImage* getBackgroundImage(ButtonState state) const;
		
		void setBackgroundColor(const Color& color, ButtonState state);
		const Color& getBackgroundColor(ButtonState state) const;
		
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
		void updateStateProperties();
		
		std::function<void()> tapHandler;
		
		ImageElement* backgroundElement;
		ImageElement* imageElement;
		TextElement* titleElement;
		
		ButtonState buttonState;
		BasicDictionary<ButtonState, String> titles;
		BasicDictionary<ButtonState, Color> titleColors;
		BasicDictionary<ButtonState, TextureImage*> images;
		BasicDictionary<ButtonState, TextureImage*> backgroundImages;
		BasicDictionary<ButtonState, Color> backgroundColors;
		BasicDictionary<ButtonState, Color> tintColors;

		bool enabled;
	};
}
