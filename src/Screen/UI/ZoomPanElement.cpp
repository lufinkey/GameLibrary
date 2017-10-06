
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
		lastScrollbarFocusMillis(0),
		horizontalScrollbarTouchID(0),
		horizontalScrollbarDragging(false),
		verticalScrollbarTouchID(0),
		verticalScrollbarDragging(false)
	{
		setClippedToFrame(true);
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
		ScreenElement::drawElements(appData, graphics);
		drawScrollbars(appData, graphics);
	}
	
	void ZoomPanElement::drawScrollbars(ApplicationData appData, Graphics graphics) const
	{
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
		
		auto frame = getFrame();
		auto scrollbarFrames = getScrollbarFrames();
		graphics.translate(frame.getTopLeft());
		graphics.setColor(fgl::Color(0,0,0,(fgl::Uint8)(255.0*visibility)));
		//draw horizontal scrollbar
		if(scrollbarFrames.first.width < frame.width)
		{
			graphics.fillRect(scrollbarFrames.first);
		}
		//draw vertical scrollbar
		if(scrollbarFrames.second.height < frame.height)
		{
			graphics.fillRect(scrollbarFrames.second);
		}
	}
	
	bool ZoomPanElement::handleTouchEvent(const TouchEvent& touchEvent)
	{
		auto frame = getFrame();
		auto scrollbarFrames = getScrollbarFrames();
		scrollbarFrames.first.x += frame.x;
		scrollbarFrames.first.y += frame.y;
		scrollbarFrames.second.x += frame.x;
		scrollbarFrames.second.y += frame.y;
		auto touchpos = touchEvent.getPosition();
		switch(touchEvent.getEventType())
		{
			case TouchEvent::EVENTTYPE_TOUCHDOWN:
			if(!horizontalScrollbarDragging && scrollbarFrames.first.contains(touchpos))
			{
				horizontalScrollbarDragging = true;
				horizontalScrollbarTouchOffset = touchpos - scrollbarFrames.first.getCenter();
				horizontalScrollbarTouchID = touchEvent.getTouchID();
				lastScrollbarFocusMillis = touchEvent.getApplicationData().getTime().getMilliseconds();
				return true;
			}
			if(!verticalScrollbarDragging && scrollbarFrames.second.contains(touchpos))
			{
				verticalScrollbarDragging = true;
				verticalScrollbarTouchOffset = touchpos - scrollbarFrames.second.getCenter();
				verticalScrollbarTouchID = touchEvent.getTouchID();
				lastScrollbarFocusMillis = touchEvent.getApplicationData().getTime().getMilliseconds();
				return true;
			}
			break;
			
			case TouchEvent::EVENTTYPE_TOUCHMOVE:
			if(frame.contains(touchpos))
			{
				lastScrollbarFocusMillis = touchEvent.getApplicationData().getTime().getMilliseconds();
			}
			if(horizontalScrollbarDragging && horizontalScrollbarTouchID==touchEvent.getTouchID())
			{
				auto newOffset = touchpos - scrollbarFrames.first.getCenter();
				auto movePortion = (newOffset.x - horizontalScrollbarTouchOffset.x)/frame.width;
				auto moveAmount = movePortion*contentSize.x;
				auto viewWidth = frame.width * zoomScale;
				contentOffset.x -= moveAmount;
				if(contentOffset.x > 0)
				{
					contentOffset.x = 0;
				}
				else if(-(contentOffset.x-viewWidth) > contentSize.x)
				{
					contentOffset.x = -(contentSize.x-viewWidth);
				}
				return true;
			}
			if(verticalScrollbarDragging && verticalScrollbarTouchID==touchEvent.getTouchID())
			{
				auto newOffset = touchpos - scrollbarFrames.second.getCenter();
				auto movePortion = (newOffset.y - verticalScrollbarTouchOffset.y)/frame.height;
				auto moveAmount = movePortion*contentSize.x;
				auto viewHeight = frame.height * zoomScale;
				contentOffset.y -= moveAmount;
				if(contentOffset.y > 0)
				{
					contentOffset.y = 0;
				}
				else if(-(contentOffset.y-viewHeight) > contentSize.y)
				{
					contentOffset.y = -(contentSize.y-viewHeight);
				}
				return true;
			}
			break;
			
			case TouchEvent::EVENTTYPE_TOUCHUP:
			case TouchEvent::EVENTTYPE_TOUCHCANCEL:
			if(horizontalScrollbarDragging && horizontalScrollbarTouchID==touchEvent.getTouchID())
			{
				horizontalScrollbarDragging = false;
				horizontalScrollbarTouchID = 0;
				horizontalScrollbarTouchOffset = Vector2d(0,0);
				return false;
			}
			if(verticalScrollbarDragging && verticalScrollbarTouchID==touchEvent.getTouchID())
			{
				verticalScrollbarDragging = false;
				verticalScrollbarTouchID = 0;
				verticalScrollbarTouchOffset = Vector2d(0,0);
				return false;
			}
			break;
		}
		return false;
	}
	
	void ZoomPanElement::otherElementHandledTouchEvent(const TouchEvent& touchEvent)
	{
		if(horizontalScrollbarDragging && horizontalScrollbarTouchID==touchEvent.getTouchID())
		{
			horizontalScrollbarDragging = false;
			horizontalScrollbarTouchID = 0;
			horizontalScrollbarTouchOffset = Vector2d(0,0);
		}
		if(verticalScrollbarDragging && verticalScrollbarTouchID==touchEvent.getTouchID())
		{
			verticalScrollbarDragging = false;
			verticalScrollbarTouchID = 0;
			verticalScrollbarTouchOffset = Vector2d(0,0);
		}
	}
	
	ApplicationData ZoomPanElement::getChildrenApplicationData(ApplicationData appData) const
	{
		appData = ScreenElement::getChildrenApplicationData(appData);
		appData.getTransform().translate(contentOffset.x*zoomScale, contentOffset.y*zoomScale);
		appData.getTransform().scale(zoomScale, zoomScale);
		return appData;
	}
	
	Graphics ZoomPanElement::getChildrenGraphics(Graphics graphics) const
	{
		auto newGraphics = ScreenElement::getChildrenGraphics(graphics);
		newGraphics.translate(contentOffset.x*zoomScale, contentOffset.y*zoomScale);
		newGraphics.scale(zoomScale, zoomScale);
		return newGraphics;
	}
	
	std::pair<RectangleD, RectangleD> ZoomPanElement::getScrollbarFrames() const
	{
		auto frame = getFrame();
		auto frameSize = frame.getSize();
		auto realSize = frameSize / zoomScale;
		Vector2d sizePortion;
		Vector2d offsetPortion;
		if(contentSize.x == 0 || contentSize.y == 0)
		{
			return {
				RectangleD(0, 1, frameSize.x, 10),
				RectangleD(1, 0, 10, frameSize.y)
			};
		}
		
		sizePortion = realSize / contentSize;
		offsetPortion = -contentOffset/contentSize;
		
		return {
			RectangleD(offsetPortion.x*frameSize.x, 1, sizePortion.x*frameSize.x, 10),
			RectangleD(1, offsetPortion.y*frameSize.y, 10, sizePortion.y*frameSize.y)
		};
	}
}
