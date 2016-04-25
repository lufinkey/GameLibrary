
#include <GameLibrary/Screen/Transition/FadeZoomTransition.h>

namespace GameLibrary
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

	void FadeZoomTransition::draw(ApplicationData appData, Graphics graphics, double progress, Drawable*element1, Drawable*element2) const
	{
		double zoom = startZoom + ((double)progress * (endZoom - startZoom));

		RectangleD e2rect = element2->getFrame();
		Vector2d e2center = Vector2d(e2rect.x+(e2rect.width/2), e2rect.y+(e2rect.height/2));

		double xOffset = (e2rect.width - (e2rect.width * zoom)) / (double)(2 * zoom);
		double yOffset = (e2rect.height - (e2rect.height * zoom)) / (double)(2 * zoom);
		byte alpha = (byte)(progress*255);

		element1->draw(appData, graphics);

		graphics.translate(xOffset+(e2center.x*zoom), yOffset+(e2center.y*zoom));
		graphics.scale(zoom, zoom);
		graphics.setAlpha(alpha);

		element2->draw(appData, graphics);
	}
}