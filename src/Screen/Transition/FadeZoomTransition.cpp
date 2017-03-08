
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

		Vector2d e2size = screen2->getSize();
		Vector2d e2center = Vector2d(e2size.x/2, e2size.y/2);

		double xOffset = (e2size.x - (e2size.x * zoom)) / (double)(2 * zoom);
		double yOffset = (e2size.y - (e2size.y * zoom)) / (double)(2 * zoom);

		screen1->draw(appData, graphics);

		graphics.translate(xOffset+(e2center.x*zoom), yOffset+(e2center.y*zoom));
		graphics.scale(zoom, zoom);
		graphics.setAlpha((float)progress);

		screen2->draw(appData, graphics);
	}
}
