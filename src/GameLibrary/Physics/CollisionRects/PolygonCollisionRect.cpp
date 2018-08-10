
#include <GameLibrary/Physics/CollisionRects/PolygonCollisionRect.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>

namespace fgl
{
	PolygonCollisionRect::PolygonCollisionRect(const String& tag, const PolygonD& polygon, const PolygonD& lastPolygon, const Vector2d& resolution)
		: PolygonCollisionRect(tag, polygon, lastPolygon.getRectangle(), resolution) {
		//
	}
	
	PolygonCollisionRect::PolygonCollisionRect(const String& tag, const PolygonD& polygon, const RectangleD& lastRect, const Vector2d& resolution)
		: CollisionRect(tag),
		polygon(polygon),
		rect(polygon.getRectangle()),
		lastRect(lastRect),
		resolution(resolution) {
		//
	}
	
	RectangleD PolygonCollisionRect::getRect() const {
		return rect;
	}
	
	RectangleD PolygonCollisionRect::getPreviousRect() const {
		return lastRect;
	}
	
	bool PolygonCollisionRect::isFilled() const {
		return false;
	}
	
	PixelIterator PolygonCollisionRect::createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const {
		return PixelIterator(Vector2u(1, 1), RectangleU(0,0,1,1), rect, loopRect, increment.x, increment.y);
	}
	
	bool PolygonCollisionRect::check(const PixelIterator& iterator) const {
		return polygon.contains(iterator.getCurrentPoint());
	}
	
	bool PolygonCollisionRect::check(const Vector2d& point) const {
		return polygon.contains(point);
	}

	Vector2d PolygonCollisionRect::getPreferredIncrement() const {
		return resolution;
	}

	void PolygonCollisionRect::draw(Graphics graphics) const {
		graphics.drawRect(rect);
		graphics.drawPolygon(polygon);
	}
}
