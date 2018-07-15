
#pragma once

#include "CollisionRect.hpp"
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/Geometry/Polygon.hpp>

namespace fgl
{
	class PolygonCollisionRect : public CollisionRect
	{
	public:
		PolygonCollisionRect(const String& tag, const PolygonD& polygon, const PolygonD& lastPolygon, const Vector2d& resolution=Vector2d(1.0, 1.0));
		PolygonCollisionRect(const String& tag, const PolygonD& polygon, const RectangleD& lastRect, const Vector2d& resolution=Vector2d(1.0, 1.0));
		
		virtual RectangleD getRect() const override;
		virtual RectangleD getPreviousRect() const override;
		virtual bool isFilled() const override;
		virtual PixelIterator createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const override;
		virtual bool check(const PixelIterator& iterator) const override;

		virtual Vector2d getPreferredIncrement() const override;

		virtual void shift(const Vector2d& shiftAmount) override;
		
		virtual void draw(Graphics graphics) const override;
		
	private:
		PolygonD polygon;
		RectangleD rect;
		RectangleD lastRect;
		Vector2d resolution;
	};
}

REGISTER_TYPE(fgl::PolygonCollisionRect, fgl::CollisionRect)
