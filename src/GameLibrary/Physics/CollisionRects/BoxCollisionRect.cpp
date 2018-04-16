
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>

namespace fgl
{
	BoxCollisionRect::BoxCollisionRect(const String& tag, const RectangleD& rect, const RectangleD& lastRect, const Vector2d& resolution)
		: CollisionRect(tag),
		rect(rect),
		lastRect(lastRect),
		boundingRect(rect),
		resolution(resolution),
		usesTransform(false)
	{
		//
	}

	BoxCollisionRect::BoxCollisionRect(const String& tag, const RectangleD& rect, const RectangleD& lastRect, double rotation, const Vector2d& origin, const Vector2d& resolution)
		: CollisionRect(tag),
		rect(rect),
		lastRect(lastRect),
		resolution(resolution),
		usesTransform(true)
	{
		srcTransform.rotate(rotation, origin);
		RectangleD relBoundingRect = srcTransform.transform(RectangleD(0,0,rect.width,rect.height));
		boundingRect = RectangleD(rect.x+relBoundingRect.x, rect.y+relBoundingRect.y, relBoundingRect.width, relBoundingRect.height);
	}

	RectangleD BoxCollisionRect::getRect() const
	{
		return boundingRect;
	}
	
	RectangleD BoxCollisionRect::getPreviousRect() const
	{
		return lastRect;
	}

	bool BoxCollisionRect::isFilled() const
	{
		if(usesTransform)
		{
			return false;
		}
		return true;
	}

	PixelIterator BoxCollisionRect::createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const
	{
		if(usesTransform)
		{
			return PixelIterator(Vector2u(1, 1), RectangleU(0, 0, 1, 1), boundingRect, loopRect, increment.x, increment.y, srcTransform, Vector2d((rect.width==0 ? 0 : 1/rect.width), (rect.height==0 ? 0 : 1/rect.height)));
		}
		else
		{
			return PixelIterator(Vector2u(1, 1), RectangleU(0, 0, 1, 1), boundingRect, loopRect, increment.x, increment.y);
		}
	}

	bool BoxCollisionRect::check(const PixelIterator& iterator) const
	{
		return (iterator.getCurrentPixelIndex()>=0);
	}

	Vector2d BoxCollisionRect::getPreferredIncrement() const
	{
		return resolution;
	}

	void BoxCollisionRect::shift(const Vector2d& shiftAmount)
	{
		rect.x += shiftAmount.x;
		rect.y += shiftAmount.y;
		boundingRect.x += shiftAmount.x;
		boundingRect.y += shiftAmount.y;
	}
}
