
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
		ButtonElement();
		ButtonElement(const RectangleD& frame);
		ButtonElement(const RectangleD& frame, const String& title, const std::function<void()>& handler);
		virtual ~ButtonElement();
		
		virtual void layoutChildElements() override;
		
		void setHandler(const std::function<void()>& handler);
		const std::function<void()>& getHandler() const;
		
		void setTitle(const String& title);
		const String& getTitle() const;
		
		void setTitleColor(const Color& titleColor);
		const Color& getTitleColor() const;
		
		void setImage(TextureImage* image);
		TextureImage* getImage() const;
		
	protected:
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		
	private:
		std::function<void()> handler;
		
		ImageElement* imageElement;
		TextElement* titleElement;
	};
}
