
#include <GameLibrary/Screen/Transition/FadeZoomTransition.hpp>
#include <GameLibrary/Screen/Screen.hpp>

namespace fgl
{
	FadeZoomTransition::FadeZoomTransition(double start, double end)
	{
		startZoom = start;
		endZoom = end;
	}

	FadeZoomTransition::~FadeZoomTransition()
	{
		//
	}

	void FadeZoomTransition::draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const
	{
		double zoom = startZoom + ((double)progress * (endZoom - startZoom));

		Vector2d e1size = screen1->getSize();

		screen1->draw(appData, graphics);
		graphics.scale(zoom, zoom, e1size.x/2, e1size.y/2);
		graphics.setAlpha((float)progress);

		screen2->draw(appData, graphics);
	}
}
