
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
		static CollisionRect* fromFrame(const String& tag, Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, const Vector2d& resolution=Vector2d(1.0,1.0));
		static CollisionRect* fromFrame(const String& tag, const TransformState& state, const TransformState& prevState, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, const Vector2d& resolution=Vector2d(1.0,1.0));
		static CollisionRect* fromFrame(const String& tag, const RectangleD& rect, const Vector2d& displacement, const ArrayList<CollisionRect*>& prevRects, const Vector2d& resolution=Vector2d(1.0,1.0));
		
		static CollisionRect* fromAnimation(const String& tag, Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal=false, bool mirroredVertical=false);
		static CollisionRect* fromAnimation(const String& tag, const TransformState& state, const TransformState& prevState, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal=false, bool mirroredVertical=false);
		
		static CollisionRect* fromPolygon(const String& tag, const PolygonD& polygon, const Vector2d& displacement, const ArrayList<CollisionRect*>& prevRects, const Vector2d& resolution=Vector2d(1.0,1.0));
		
		static size_t findMatchingRectIndex(const ArrayList<CollisionRect*>& collisionRects, const String& tag);
		static RectangleD getMatchingRect(const ArrayList<CollisionRect*>& collisionRects, const String& tag, const RectangleD& currentRect, const Vector2d& displacement);
	};
}
