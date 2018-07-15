
#pragma once

#include <functional>
#include "TouchElement.hpp"

namespace fgl
{
	class CheckboxElement : public TouchElement
	{
	public:
		CheckboxElement();
		CheckboxElement(const RectangleD& frame);
		
		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;
		
		void setToggle(bool toggle);
		bool getToggle() const;
		
		void setToggleHandler(const std::function<void(bool)>& toggleHandler);
		const std::function<void(bool)>& getToggleHandler() const;
		
	protected:
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		
	private:
		std::function<void(bool)> toggleHandler;
		bool toggle;
	};
}

REGISTER_TYPE(fgl::CheckboxElement, fgl::TouchElement)
