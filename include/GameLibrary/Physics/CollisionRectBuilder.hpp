
#pragma once

#include <functional>
#include "Collidable.hpp"
#include <GameLibrary/Actor/Animation.hpp>

namespace fgl
{
	class CollisionRectBuilder
	{
	public:
		static ArrayList<CollisionRect*> fromFrame(Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, Vector2d size, Vector2d origin, Vector2d resolution=Vector2d(1.0,1.0));
		static ArrayList<CollisionRect*> fromAnimation(Collidable* collidable, const ArrayList<CollisionRect*>& prevRects, Vector2d size, Vector2d origin, Animation* animation, size_t frameIndex, bool mirroredHorizontal=false, bool mirroredVertical=false);
	};
}
