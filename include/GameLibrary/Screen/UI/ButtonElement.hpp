
#pragma once

#include <functional>
#include "TouchElement.hpp"
#include "ImageElement.hpp"
#include "TextElement.hpp"

class setButtonState;
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
			BUTTONSTATE_SELECTED,
			BUTTONSTATE_DISABLED,
		} ButtonState;
		
		ButtonElement();
		ButtonElement(const RectangleD& frame);
		ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& tapHandler);
		
		virtual void draw(ApplicationData appData, Graphics graphics) const override;
		
		void setTapHandler(const std::function<void()>& tapHandler);
		const std::function<void()>& getTapHandler() const;
		
		ButtonState getButtonState() const;

		void setEnabled(bool enabled);
		bool isEnabled() const;
		
		void setSelected(bool selected);
		bool isSelected() const;
		
		bool isPressed() const;
		
		void setTitle(const String& title, ButtonState state);
		void setTitle(std::nullptr_t, ButtonState state);
		String getTitle(ButtonState state) const;
		
		void setTitleColor(const Color& titleColor, ButtonState state);
		void setTitleColor(std::nullptr_t, ButtonState state);
		Color getTitleColor(ButtonState state) const;
		
		void setTintColor(const Color& tintColor, ButtonState state);
		void setTintColor(std::nullptr_t, ButtonState state);
		Color getTintColor(ButtonState state) const;
		
		void setImage(TextureImage* image, ButtonState state);
		void setImage(std::nullptr_t, ButtonState state);
		TextureImage* getImage(ButtonState state) const;

		void setBackgroundImage(TextureImage* image, ButtonState state);
		void setBackgroundImage(std::nullptr_t, ButtonState state);
		TextureImage* getBackgroundImage(ButtonState state) const;
		
		void setBackgroundColor(const Color& color, ButtonState state);
		void setBackgroundColor(std::nullptr_t, ButtonState state);
		Color getBackgroundColor(ButtonState state) const;
		
		TextElement* getTitleElement() const;
		ImageElement* getImageElement() const;
		
	protected:
		virtual void onLayoutChildElements() override;
		
		virtual void onMouseEnter(unsigned int mouseIndex) override;
		virtual void onMouseLeave(unsigned int mouseIndex) override;
		
		virtual void onTouchDown(const TouchEvent& touchEvent) override;
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		virtual void onTouchUpOutside(const TouchEvent& touchEvent) override;
		virtual void onTouchCancel(const TouchEvent& touchEvent) override;
		
	private:
		void updateButtonState();
		void setButtonState(ButtonState buttonState);
		void updateStateProperties();
		
		template<typename T>
		const T& getProperty(const BasicDictionary<ButtonState, T>& dict, ButtonState state, const T& defaultValue) const
		{
			fgl::ArrayList<ButtonState> states;
			if(selected)
			{
				if(state==BUTTONSTATE_DISABLED)
				{
					states = {state, BUTTONSTATE_NORMAL};
				}
				else
				{
					states = {state, BUTTONSTATE_SELECTED, BUTTONSTATE_NORMAL};
				}
			}
			else
			{
				states = {state, BUTTONSTATE_NORMAL};
			}
			for(auto& state : states)
			{
				try
				{
					return dict.get(state);
				}
				catch(const DictionaryKeyNotFoundException&)
				{
					// do nothing
				}
			}
			return defaultValue;
		}
		
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
		bool selected;
		bool pressed;
		unsigned int pressedTouchID;
	};
}
