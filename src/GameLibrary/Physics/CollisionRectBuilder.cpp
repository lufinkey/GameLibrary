
#include <GameLibrary/Physics/CollisionRectBuilder.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PixelCollisionRect.hpp>

namespace fgl
{
	ArrayList<CollisionRect*> CollisionRectBuilder::fromFrame(Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, Vector2d size, Vector2d origin, Vector2d resolution)
	{
		auto prevTransformState = collidable->getPreviousTransformState();
		auto transformState = collidable->getTransformState();
		auto displacement = transformState.position - prevTransformState.position;
		auto rect = RectangleD(transformState.position.x-origin.x, transformState.position.y-origin.y, size.x, size.y);
		auto lastRect = getMatchingRect(prevRects, "all", rect, displacement);
		if(transformState.rotation!=0.0)
		{
			return {new BoxCollisionRect("all", rect, lastRect, transformState.rotation, origin, resolution)};
		}
		return {new BoxCollisionRect("all", rect, lastRect, resolution)};
	}
	
	ArrayList<CollisionRect*> CollisionRectBuilder::fromAnimation(Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, Vector2d size, Vector2d origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal, bool mirroredVertical)
	{
		auto prevTransformState = collidable->getPreviousTransformState();
		auto transformState = collidable->getTransformState();
		auto displacement = transformState.position - prevTransformState.position;
		auto img = animation->getImage(frameIndex);
		auto srcRect = animation->getImageSourceRect(frameIndex);
		auto rect = RectangleD(transformState.position.x-origin.x, transformState.position.y-origin.y, size.x, size.y);
		auto lastRect = getMatchingRect(prevRects, "all", rect, displacement);
		if(transformState.rotation!=0.0)
		{
			return {new PixelCollisionRect("all", rect, lastRect, srcRect, transformState.rotation, origin, img, mirroredHorizontal, mirroredVertical)};
		}
		return {new PixelCollisionRect("all", rect, lastRect, srcRect, img, mirroredHorizontal, mirroredVertical)};
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
