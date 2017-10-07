
#include <GameLibrary/Screen/Transition/PopoverTransition.hpp>
#include <GameLibrary/Screen/Screen.hpp>

namespace fgl
{
	PopoverTransition::PopoverTransition(PopoverDirection dir, bool fades)
	{
		if(dir == POPOVER_UP || dir == POPOVER_RIGHT || dir == POPOVER_DOWN || dir == POPOVER_LEFT)
		{
			direction = dir;
		}
		else
		{
			direction = POPOVER_UP;
		}
		fade = fades;
	}
	
	PopoverTransition::~PopoverTransition()
	{
		//
	}
	
	void PopoverTransition::draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const
	{
		Vector2d e1size = screen1->getSize();
		Vector2d e2size = screen2->getSize();
		
		double xOff = (e2size.x - e1size.x)/2;
		double yOff = (e2size.y - e1size.y)/2;
		
		double translationX = xOff;
		double translationY = yOff;
		switch(direction)
		{
			case POPOVER_UP:
			translationY += (e1size.y*(1-progress));
			break;
			
			case POPOVER_DOWN:
			translationY += -(e1size.y + (e1size.y*progress));
			break;
			
			case POPOVER_LEFT:
			translationX += -(e1size.x - (e1size.x*progress));
			break;
			
			case POPOVER_RIGHT:
			translationX += (e1size.x * (1-progress));
			break;
		}
		
		screen1->draw(appData, graphics);
		
		if(fade)
		{
			graphics.compositeAlpha((float)progress);
		}
		graphics.translate((double)translationX, (double)translationY);
		
		screen2->draw(appData, graphics);
	}
}
