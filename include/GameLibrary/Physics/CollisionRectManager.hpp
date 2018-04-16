
#pragma once

#include <functional>
#include "Collidable.hpp"
#include <GameLibrary/Actor/Animation.hpp>

namespace fgl
{
	class CollisionRectManager
	{
	public:
		CollisionRectManager();
		~CollisionRectManager();
		
		ArrayList<CollisionRect*> createRectsFromFrame(Collidable* collidable, Vector2d size, Vector2d origin, Vector2d scale=Vector2d(1.0,1.0));
		ArrayList<CollisionRect*> createRectsFromAnimation(Collidable* collidable, Vector2d size, Vector2d origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal=false, bool mirroredVertical=false);
		void updateRects(const std::function<ArrayList<CollisionRect*>()>& rectCreator);
		
		const ArrayList<CollisionRect*>& getCollisionRects() const;
		
	private:
		ArrayList<CollisionRect*> collisionRects;
	};
}
