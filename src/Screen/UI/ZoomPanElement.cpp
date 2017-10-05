
#include <GameLibrary/Screen/UI/ZoomPanElement.hpp>
#include <GameLibrary/IO/Console.hpp>

namespace fgl
{
	#define SCROLLBAR_VISIBLE_TIME 1800
	#define SCROLLBAR_FADE_TIME 1200
	
	ZoomPanElement::ZoomPanElement() : ZoomPanElement(RectangleD(0,0,0,0))
	{
		//
	}
	
	ZoomPanElement::ZoomPanElement(const RectangleD& frame)
		: ScreenElement(frame),
		contentOffset(0,0),
		contentSize(0,0),
		zoomScale(1),
		lastScrollbarFocusMillis(0)
	{
		//
	}
	
	void ZoomPanElement::setContentOffset(const Vector2d& offset)
	{
		contentOffset = offset;
	}
	
	const Vector2d& ZoomPanElement::getContentOffset() const
	{
		return contentOffset;
	}
	
	void ZoomPanElement::setContentSize(const Vector2d& size)
	{
		contentSize = size;
	}
	
	const Vector2d& ZoomPanElement::getContentSize() const
	{
		return contentSize;
	}
	
	void ZoomPanElement::setZoomScale(double zoom)
	{
		zoomScale = zoom;
	}
	
	double ZoomPanElement::getZoomScale() const
	{
		return zoomScale;
	}
	
	void ZoomPanElement::zoomOnPoint(const Vector2d&point, double zoom)
	{
		RectangleD frame = getFrame();
		double offsetX = (frame.width/2)*zoom;
		double offsetY = (frame.height/2)*zoom;
		contentOffset.x = point.x - offsetX;
		contentOffset.y = point.y - offsetY;
		zoomScale = zoom;
	}
	
	void ZoomPanElement::zoomOnPointInFrame(const Vector2d&point, double zoom)
	{
		Vector2d fixedPoint((point.x-contentOffset.x)/zoom, (point.y-contentOffset.y)/zoom);
		zoomOnPoint(fixedPoint, zoom);
	}
	
	void ZoomPanElement::drawElements(ApplicationData appData, Graphics graphics) const
	{
		auto elementGraphics = graphics;
		auto frame = getFrame();
		elementGraphics.clip(RectangleD(frame.x,frame.y,frame.width,frame.height));
		elementGraphics.translate(contentOffset.x*zoomScale, contentOffset.y*zoomScale);
		elementGraphics.scale(zoomScale, zoomScale);
		ScreenElement::drawElements(appData, elementGraphics);
		
		drawScrollbars(appData, graphics);
	}
	
	void ZoomPanElement::drawScrollbars(ApplicationData appData, Graphics graphics) const
	{
		auto frame = getFrame();
		auto frameSize = frame.getSize();
		auto realSize = frameSize / zoomScale;
		fgl::Vector2d sizePortion;
		fgl::Vector2d offsetPortion;
		if(contentSize.x != 0 && contentSize.y != 0)
		{
			sizePortion = realSize / contentSize;
			offsetPortion = -contentOffset/contentSize;
		}
		
		double visibility = 0;
		auto currentTimeMillis = appData.getTime().getMilliseconds();
		auto timeSinceLastFocus = currentTimeMillis - lastScrollbarFocusMillis;
		if(timeSinceLastFocus < SCROLLBAR_VISIBLE_TIME)
		{
			visibility = 0.7;
		}
		else if(timeSinceLastFocus < (SCROLLBAR_VISIBLE_TIME+SCROLLBAR_FADE_TIME))
		{
			visibility = 0.7 - (((double)(timeSinceLastFocus - SCROLLBAR_VISIBLE_TIME) / (double)SCROLLBAR_FADE_TIME)*0.7);
		}
		
		graphics.translate(frame.getTopLeft());
		graphics.setColor(fgl::Color(0,0,0,(fgl::Uint8)(255.0*visibility)));
		if(sizePortion.x > 0 && sizePortion.x < 1.0)
		{
			graphics.fillRect(fgl::RectangleD(offsetPortion.x*frameSize.x, 1, sizePortion.x*frameSize.x, 10));
		}
		if(sizePortion.y > 0 && sizePortion.y < 1.0)
		{
			graphics.fillRect(fgl::RectangleD(1, offsetPortion.y*frameSize.y, 10, sizePortion.y*frameSize.y));
		}
	}
	
	bool ZoomPanElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		switch(touchEvent.getEventType())
		{
			case TouchEvent::EVENTTYPE_TOUCHDOWN:
			break;
			
			case TouchEvent::EVENTTYPE_TOUCHMOVE:
			lastScrollbarFocusMillis = touchEvent.getApplicationData().getTime().getMilliseconds();
			break;
			
			case TouchEvent::EVENTTYPE_TOUCHUP:
			break;
			
			case TouchEvent::EVENTTYPE_TOUCHCANCEL:
			break;
		}
		return false;
	}
	
	void ZoomPanElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		//
	}
}
