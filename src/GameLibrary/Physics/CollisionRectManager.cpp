
#include <GameLibrary/Physics/CollisionRectManager.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PixelCollisionRect.hpp>

namespace fgl
{
	CollisionRectManager::CollisionRectManager()
	{
		//
	}
	
	CollisionRectManager::~CollisionRectManager()
	{
		for(auto collisionRect : collisionRects)
		{
			delete collisionRect;
		}
	}
	
	const ArrayList<CollisionRect*>& CollisionRectManager::getCollisionRects() const
	{
		return collisionRects;
	}
	
	void CollisionRectManager::updateRects(const std::function<ArrayList<CollisionRect*>()>& rectCreator)
	{
		auto previousRects = collisionRects;
		collisionRects = rectCreator();
		for(auto collisionRect : previousRects)
		{
			delete collisionRect;
		}
	}
	
	ArrayList<CollisionRect*> CollisionRectManager::createRectsFromFrame(Collidable* collidable, Vector2d size, Vector2d origin, Vector2d resolution)
	{
		auto prevTransformState = collidable->getPreviousTransformState();
		auto transformState = collidable->getTransformState();
		auto positionDiff = transformState.position - prevTransformState.position;
		auto rect = RectangleD(transformState.position.x-origin.x, transformState.position.y-origin.y, size.x, size.y);
		auto lastRect = rect;
		lastRect.x -= positionDiff.x;
		lastRect.y -= positionDiff.y;
		size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
			if(rect->getTag()=="all")
			{
				return true;
			}
			return false;
		});
		if(matchingRectIndex!=-1)
		{
			lastRect = collisionRects[matchingRectIndex]->getRect();
		}
		if(transformState.rotation!=0.0)
		{
			return {new BoxCollisionRect("all", rect, lastRect, transformState.rotation, origin, resolution)};
		}
		return {new BoxCollisionRect("all", rect, lastRect, resolution)};
	}
	
	ArrayList<CollisionRect*> CollisionRectManager::createRectsFromAnimation(Collidable* collidable, Vector2d size, Vector2d origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal, bool mirroredVertical)
	{
		auto prevTransformState = collidable->getPreviousTransformState();
		auto transformState = collidable->getTransformState();
		auto positionDiff = transformState.position - prevTransformState.position;
		auto img = animation->getImage(frameIndex);
		auto srcRect = animation->getImageSourceRect(frameIndex);
		auto rect = RectangleD(transformState.position.x-origin.x, transformState.position.y-origin.y, size.x, size.y);
		auto lastRect = rect;
		lastRect.x -= positionDiff.x;
		lastRect.y -= positionDiff.y;
		size_t matchingRectIndex = collisionRects.indexWhere([](CollisionRect* const & rect) -> bool {
			if(rect->getTag()=="all")
			{
				return true;
			}
			return false;
		});
		if(matchingRectIndex!=-1)
		{
			lastRect = collisionRects[matchingRectIndex]->getRect();
		}
		if(transformState.rotation!=0.0)
		{
			return {new PixelCollisionRect("all", rect, lastRect, srcRect, transformState.rotation, origin, img, mirroredHorizontal, mirroredVertical)};
		}
		return {new PixelCollisionRect("all", rect, lastRect, srcRect, img, mirroredHorizontal, mirroredVertical)};
	}
}
