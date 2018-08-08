
#pragma once

#include <functional>
#include "Collidable.hpp"
#include <GameLibrary/Animation/Animation.hpp>

namespace fgl
{
	class CollisionRectBuilder
	{
	private:
		CollisionRectBuilder() = delete;
		
	public:
		static const CollisionRect* fromRect(const String& tag, const RectangleD& rect, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& resolution={1.0, 1.0 });
		static const CollisionRect* fromRect(const String& tag, const RectangleD& rect, const ArrayList<const CollisionRect*>& prevRects, double rotation, const Vector2d& resolution={ 1.0, 1.0 });
		
		static const CollisionRect* fromAnimation(const String& tag, Animation* animation, size_t frameIndex, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& origin, bool mirroredHorizontal=false, bool mirroredVertical=false);
		static const CollisionRect* fromAnimation(const String& tag, Animation* animation, size_t frameIndex, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& origin, const Vector2d& size, double rotation, bool mirroredHorizontal=false, bool mirroredVertical=false);
		
		static const CollisionRect* fromPolygon(const String& tag, const PolygonD& polygon, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& resolution=Vector2d(1.0,1.0));
		
		static size_t findMatchingRectIndex(const ArrayList<const CollisionRect*>& collisionRects, const String& tag);
		static RectangleD getMatchingRect(const ArrayList<const CollisionRect*>& collisionRects, const String& tag, const RectangleD& currentRect);
	};
}
