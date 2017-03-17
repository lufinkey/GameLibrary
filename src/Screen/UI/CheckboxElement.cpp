
#include <GameLibrary/Screen/UI/CheckboxElement.hpp>

namespace fgl
{
	CheckboxElement::CheckboxElement() : CheckboxElement(RectangleD(0,0,18,18))
	{
		//
	}
	
	CheckboxElement::CheckboxElement(const RectangleD& frame) : TouchElement(frame),
		toggle(false)
	{
		setBorderWidth(1);
	}
	
	void CheckboxElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		if(toggle)
		{
			RectangleD frame = getFrame();
			double checkAxisX = frame.x+(frame.width*0.45);
			double checkAxisY = frame.y+(3*frame.height/4);
			graphics.setColor(Color::BLACK);
			graphics.drawLine(checkAxisX-(frame.width*0.2), checkAxisY-(frame.height*0.2), checkAxisX, checkAxisY);
			graphics.drawLine(checkAxisX, checkAxisY, checkAxisX+(frame.width*0.3), checkAxisY-(frame.height*0.5));
		}
	}
	
	void CheckboxElement::setToggle(bool toggle_arg)
	{
		toggle = toggle_arg;
	}
	
	bool CheckboxElement::getToggle() const
	{
		return toggle;
	}
	
	void CheckboxElement::setToggleHandler(const std::function<void(bool)>& toggleHandler_arg)
	{
		toggleHandler = toggleHandler_arg;
	}
	
	const std::function<void(bool)>& CheckboxElement::getToggleHandler() const
	{
		return toggleHandler;
	}
	
	void CheckboxElement::onTouchUpInside(const fgl::ScreenElement::TouchEvent& touchEvent)
	{
		setToggle(!toggle);
		if(toggleHandler)
		{
			toggleHandler(toggle);
		}
	}
}
