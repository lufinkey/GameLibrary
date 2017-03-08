
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>

namespace fgl
{
	class ZoomPanElement : public ScreenElement
	{
	public:
		ZoomPanElement();
		explicit ZoomPanElement(const RectangleD&frame);
		
		void setContentOffset(const Vector2d&offset);
		const Vector2d& getContentOffset() const;
		
		void setZoomScale(double zoomScale);
		double getZoomScale() const;
		
		void zoomOnPoint(const Vector2d&point, double zoomScale);
		void zoomOnPointInFrame(const Vector2d&point, double zoomScale);

		//TODO implement private touch event handling functions, and pass transformed appData to child elements
		
	private:
		/*! \copydoc fgl::ScreenElement::drawElements(fgl::ApplicationData,fgl::Graphics)const*/
		virtual void drawElements(ApplicationData appData, Graphics graphics) const override;

		Vector2d contentOffset;
		double zoomScale;
	};
}
