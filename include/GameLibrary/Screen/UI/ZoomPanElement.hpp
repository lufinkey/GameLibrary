
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>

namespace fgl
{
	class ZoomPanElement : public ScreenElement
	{
	public:
		ZoomPanElement();
		explicit ZoomPanElement(const RectangleD& frame);
		
		void setContentOffset(const Vector2d& offset);
		const Vector2d& getContentOffset() const;
		
		void setContentSize(const Vector2d& size);
		const Vector2d& getContentSize() const;
		
		void setZoomScale(double zoomScale);
		double getZoomScale() const;
		
		void zoomOnPoint(const Vector2d& point, double zoomScale);
		void zoomOnPointInFrame(const Vector2d& point, double zoomScale);

		//TODO implement private touch event handling functions, and pass transformed appData to child elements
		
	protected:
		/*! \copydoc fgl::ScreenElement::drawElements(fgl::ApplicationData,fgl::Graphics)const*/
		virtual void drawElements(ApplicationData appData, Graphics graphics) const override;
		
		virtual void drawScrollbars(ApplicationData appData, Graphics graphics) const;
		
		/*! \copydoc fgl::ScreenElement::handleTouchEvent(const fgl::ScreenElement::TouchEvent&)*/
		virtual bool handleTouchEvent(const TouchEvent& touchEvent) override;
		/*! \copydoc fgl::ScreenElement::otherElementHandledTouchEvent(const fgl::ScreenElement::TouchEvent&)*/
		virtual void otherElementHandledTouchEvent(const TouchEvent& touchEvent) override;
		
		/*! \copydoc fgl::ScreenElement::getChildrenApplicationData(fgl::ApplicationData)*/
		virtual ApplicationData getChildrenApplicationData(ApplicationData appData) const override;
		/*! \copydoc fgl::ScreenElement::getChildrenGraphics(fgl::Graphics)*/
		virtual Graphics getChildrenGraphics(Graphics graphics) const override;
		
		/*! Gets the horizontal and vertical scrollbar frames
			\returns a pair containing the horizontal and vertical scrollbar frames, respectively */
		std::pair<RectangleD, RectangleD> getScrollbarFrames() const;

	private:
		Vector2d contentOffset;
		Vector2d contentSize;
		double zoomScale;
		
		long long lastScrollbarFocusMillis;
		
		unsigned int horizontalScrollbarTouchID;
		Vector2d horizontalScrollbarTouchOffset;
		bool horizontalScrollbarDragging;
		
		unsigned int verticalScrollbarTouchID;
		Vector2d verticalScrollbarTouchOffset;
		bool verticalScrollbarDragging;
	};
}

REGISTER_TYPE(fgl, ZoomPanElement, fgl::ScreenElement)
