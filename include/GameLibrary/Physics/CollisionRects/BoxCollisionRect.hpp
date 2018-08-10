
#pragma once

#include "CollisionRect.hpp"
#include <GameLibrary/Utilities/Geometry/Transform.hpp>

namespace fgl
{
	class BoxCollisionRect : public CollisionRect
	{
	public:
		BoxCollisionRect(const String& tag, const RectangleD& rect, const RectangleD& lastRect, const Vector2d& resolution=Vector2d(1.0, 1.0));

		virtual RectangleD getRect() const override;
		virtual RectangleD getPreviousRect() const override;
		virtual bool isFilled() const override;
		virtual PixelIterator createPixelIterator(const fgl::RectangleD& loopRect, const Vector2d& increment) const override;
		virtual bool check(const PixelIterator& iterator) const override;
		virtual bool check(const Vector2d& point) const override;

		virtual Vector2d getPreferredIncrement() const override;
		
		virtual void draw(Graphics graphics) const override;

	private:
		RectangleD rect;
		RectangleD lastRect;
		Vector2d resolution;
	};
}
