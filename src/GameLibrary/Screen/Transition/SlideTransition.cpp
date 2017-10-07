
#include <GameLibrary/Screen/Transition/SlideTransition.hpp>
#include <GameLibrary/Screen/Screen.hpp>

namespace fgl
{
	SlideTransition::SlideTransition(SlideDirection dir)
	{
		if(dir == SLIDE_UP || dir == SLIDE_RIGHT || dir == SLIDE_DOWN || dir == SLIDE_LEFT)
		{
			direction = dir;
		}
		else
		{
			direction = SLIDE_LEFT;
		}
	}
	
	SlideTransition::~SlideTransition()
	{
		//
	}
	
	void SlideTransition::draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const
	{
		Graphics screen1Graphics(graphics);
		Graphics screen2Graphics(graphics);
		
		double h1 = 0;
		double h2 = 0;
		double w1 = 0;
		double w2 = 0;

		Vector2d e1size = screen1->getSize();
		Vector2d e2size = screen2->getSize();

		Vector2d mainSize = Vector2d(Math::max(e1size.x, e2size.x), Math::max(e2size.x, e2size.y));
		
		switch(direction)
		{
			case SLIDE_UP:
			{
				h1 = -((double)(progress * mainSize.y));
				h2 = mainSize.y + h1;
			}
			break;
			
			case SLIDE_RIGHT:
			{
				w1 = (double)(progress * mainSize.x);
				w2 = -mainSize.x + w1;
			}
			break;
			
			case SLIDE_DOWN:
			{
				h1 = (double)(progress * mainSize.y);
				h2 = -mainSize.y + h1;
			}
			break;
			
			case SLIDE_LEFT:
			{
				w1 = -((double)(progress * mainSize.x));
				w2 = mainSize.x + w1;
			}
			break;
		}
		
		screen1Graphics.translate((double)w1, (double)h1);
		screen2Graphics.translate((double)w2, (double)h2);
		
		screen1->draw(appData, screen1Graphics);
		screen2->draw(appData, screen2Graphics);
	}
}
