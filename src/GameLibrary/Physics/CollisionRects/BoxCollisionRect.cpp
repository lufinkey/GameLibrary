
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>

namespace fgl
{
	BoxCollisionRect::BoxCollisionRect(const String& tag, const RectangleD& rect, const RectangleD& lastRect, const Vector2d& resolution)
		: CollisionRect(tag),
		rect(rect),
		lastRect(lastRect),
		resolution(resolution) {
		//
	}

	RectangleD BoxCollisionRect::getRect() const {
		return rect;
	}
	
	RectangleD BoxCollisionRect::getPreviousRect() const {
		return lastRect;
	}

	bool BoxCollisionRect::isFilled() const {
		return true;
	}

	PixelIterator BoxCollisionRect::createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const {
		return PixelIterator(Vector2u(1, 1), RectangleU(0, 0, 1, 1), rect, loopRect, increment.x, increment.y);
	}

	bool BoxCollisionRect::check(const PixelIterator& iterator) const {
		return (iterator.getCurrentPixelIndex()>=0);
	}

	Vector2d BoxCollisionRect::getPreferredIncrement() const {
		return resolution;
	}

	void BoxCollisionRect::draw(Graphics graphics) const {
		graphics.drawRect(rect);
	}
}
