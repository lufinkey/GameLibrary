
#include <GameLibrary/Physics/CollisionRectBuilder.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PixelCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PolygonCollisionRect.hpp>

namespace fgl
{
	CollisionRect* CollisionRectBuilder::fromFrame(const String& tag, Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, const Vector2d& resolution)
	{
		return fromFrame(tag, collidable->getTransformState(), collidable->getPreviousTransformState(), prevRects, size, origin, resolution);
	}
	
	CollisionRect* CollisionRectBuilder::fromFrame(const String& tag, const TransformState& state, const TransformState& prevState, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, const Vector2d& resolution)
	{
		auto displacement = state.position - prevState.position;
		auto rect = RectangleD(state.position.x-origin.x, state.position.y-origin.y, size.x, size.y);
		auto lastRect = getMatchingRect(prevRects, tag, rect, displacement);
		if(state.rotation!=0.0)
		{
			return new BoxCollisionRect(tag, rect, lastRect, state.rotation, origin, resolution);
		}
		return new BoxCollisionRect(tag, rect, lastRect, resolution);
	}
	
	CollisionRect* CollisionRectBuilder::fromFrame(const String& tag, const RectangleD& rect, const Vector2d& displacement, const ArrayList<CollisionRect*>& prevRects, const Vector2d& resolution)
	{
		auto lastRect = getMatchingRect(prevRects, tag, rect, displacement);
		return new BoxCollisionRect(tag, rect, lastRect, resolution);
	}
	
	CollisionRect* CollisionRectBuilder::fromAnimation(const String& tag, Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal, bool mirroredVertical)
	{
		return fromAnimation(tag, collidable->getTransformState(), collidable->getPreviousTransformState(), prevRects, size, origin, animation, frameIndex, mirroredHorizontal, mirroredVertical);
	}
	
	CollisionRect* CollisionRectBuilder::fromAnimation(const String& tag, const TransformState& state, const TransformState& prevState, const ArrayList<CollisionRect*>& prevRects, const Vector2d& size, const Vector2d& origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal, bool mirroredVertical)
	{
		if(animation == nullptr)
		{
			return {};
		}
		auto displacement = state.position - prevState.position;
		auto& frame = animation->getFrame(frameIndex);
		auto img = frame.getImage();
		auto srcRect = frame.getSourceRect();
		auto rect = RectangleD(state.position.x-origin.x, state.position.y-origin.y, size.x, size.y);
		auto lastRect = getMatchingRect(prevRects, tag, rect, displacement);
		if(state.rotation!=0.0)
		{
			return new PixelCollisionRect(tag, rect, lastRect, srcRect, state.rotation, origin, img, mirroredHorizontal, mirroredVertical);
		}
		return new PixelCollisionRect(tag, rect, lastRect, srcRect, img, mirroredHorizontal, mirroredVertical);
	}
	
	CollisionRect* CollisionRectBuilder::fromPolygon(const String& tag, const PolygonD& polygon, const Vector2d& displacement, const ArrayList<CollisionRect*>& prevRects, const Vector2d& resolution)
	{
		auto rect = polygon.getRectangle();
		auto lastRect = getMatchingRect(prevRects, tag, rect, displacement);
		if(rect.toPolygon() == polygon)
		{
			// polygon is a rectangle, just make a box collidable
			return new BoxCollisionRect(tag, rect, lastRect, resolution);
		}
		return new PolygonCollisionRect(tag, polygon, lastRect, resolution);
	}
	
	size_t CollisionRectBuilder::findMatchingRectIndex(const ArrayList<CollisionRect*>& collisionRects, const String& tag)
	{
		return collisionRects.indexWhere([tag](CollisionRect* const & rect) -> bool {
			if(rect->getTag()==tag)
			{
				return true;
			}
			return false;
		});
	}
	
	RectangleD CollisionRectBuilder::getMatchingRect(const ArrayList<CollisionRect*>& collisionRects, const String& tag, const RectangleD& currentRect, const Vector2d& displacement)
	{
		size_t matchingRectIndex = findMatchingRectIndex(collisionRects, tag);
		if(matchingRectIndex==(size_t)-1)
		{
			auto rect = currentRect;
			rect.x -= displacement.x;
			rect.y -= displacement.y;
			return rect;
		}
		return collisionRects[matchingRectIndex]->getRect();
	}
}
