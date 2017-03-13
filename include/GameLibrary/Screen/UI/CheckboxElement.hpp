
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
		
		void setHandler(const std::function<void(bool)>& handler);
		const std::function<void(bool)>& getHandler() const;
		
	protected:
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		
	private:
		std::function<void(bool)> handler;
		bool toggle;
	};
}
