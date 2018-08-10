
#pragma once

#include <GameLibrary/Graphics/PixelIterator.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>

namespace fgl
{
	class Graphics;
	class Collidable;
	
	class CollisionRect
	{
	public:
		CollisionRect(const String& tag);
		virtual ~CollisionRect();

		const String& getTag() const;
		virtual RectangleD getRect() const = 0;
		virtual RectangleD getPreviousRect() const = 0;
		virtual bool isFilled() const = 0;
		virtual PixelIterator createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const = 0;
		virtual bool check(const PixelIterator& iterator) const = 0;
		virtual bool check(const Vector2d& point) const = 0;
		
		virtual Vector2d getPreferredIncrement() const = 0;
		
		virtual void draw(Graphics graphics) const;

		//offset is rect2's offset to avoid rect1
		static Vector2d getCollisionOffset(const Collidable* collidable1, const CollisionRect* collisionRect1, const Collidable* collidable2, const CollisionRect* collisionRect2);
		static bool checkCollision(const Collidable* collidable1, const CollisionRect* collisionRect1, const Collidable* collidable2, const CollisionRect* collisionRect2);
		static bool checkCollision(const Collidable* collidable1, const CollisionRect* collisionRect1, const RectangleD& rect2);

	private:
		static Vector2d getFilledCollisionOffset(const Collidable* collidable1, const CollisionRect* collisionRect1, const Collidable* collidable2, const CollisionRect* collisionRect2);
		static Vector2d getPixelOnFilledCollisionOffset(const Collidable* collidable1, const CollisionRect* pixelRect, const Collidable* collidable2, const CollisionRect* filledRect);
		static Vector2d getPixelCollisionOffset(const Collidable* collidable1, const CollisionRect* collisionRect1, const Collidable* collidable2, const CollisionRect* collisionRect2);
		
		static bool checkPixelOnFilledCollision(const Collidable* collidable1, const CollisionRect* pixelRect, const RectangleD& filledRect);
		static bool checkPixelCollision(const Collidable* collidable1, const CollisionRect* collisionRect1, const Collidable* collidable2, const CollisionRect* collisionRect2);

		String tag;
	};
}
