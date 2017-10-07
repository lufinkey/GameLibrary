
#include <GameLibrary/Screen/Transition/FadeColorTransition.hpp>
#include <GameLibrary/Screen/Screen.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	FadeColorTransition::FadeColorTransition(const Color&c, double frozenPortn)
	{
		if(frozenPortn < 0 || frozenPortn > 1)
		{
			throw IllegalArgumentException("frozenPortion", "must be a value between 0 and 1");
		}
		color = c;
		frozenPortion = frozenPortn;
	}
	
	FadeColorTransition::~FadeColorTransition()
	{
		//
	}
	
	void FadeColorTransition::draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const
	{
		double firstPart = (1-frozenPortion)/2;
		double secondPart = 1-firstPart;
		if (progress < firstPart)
		{
			double alpha = ((double)progress / firstPart);
			
			Vector2d e1size = screen1->getSize();
			Vector2d e1center = Vector2d(e1size.x/2, e1size.y/2);
			
			screen1->draw(appData, graphics);
			
			graphics.setColor(color);
			graphics.compositeAlpha((float)alpha);
			graphics.fillRect(e1center.x-(e1size.x/2), e1center.y-(e1size.y/2), e1size.x, e1size.y);
		}
		else if(progress > secondPart)
		{
			double alpha = 1 - (((double)progress - secondPart) / firstPart);

			Vector2d e2size = screen2->getSize();
			Vector2d e2center = Vector2d(e2size.x/2, e2size.y/2);
			
			screen2->draw(appData, graphics);
			
			graphics.setColor(color);
			graphics.compositeAlpha((float)alpha);
			graphics.fillRect(e2center.x-(e2size.x/2), e2center.y-(e2size.y/2), e2size.x, e2size.y);
		}
		else
		{
			Vector2d e1size = screen1->getSize();
			Vector2d e1center = Vector2d(e1size.x/2, e1size.y/2);
			
			graphics.setColor(color);
			graphics.fillRect(e1center.x-(e1size.x/2), e1center.y-(e1size.y/2), e1size.x, e1size.y);
		}
	}
}
