
#pragma once

#include <GameLibrary/Graphics/PixelIterator.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class Graphics;
	
	class CollisionRect
	{
	public:
		CollisionRect(const String& tag);
		virtual ~CollisionRect();

		const String& getTag() const;
		Vector2d getCenter() const;
		virtual RectangleD getRect() const = 0;
		virtual RectangleD getPreviousRect() const = 0;
		virtual bool isFilled() const = 0;
		virtual PixelIterator createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const = 0;
		virtual bool check(const PixelIterator& iterator) const = 0;
		
		Vector2d getTotalVelocity() const;

		virtual Vector2d getPreferredIncrement() const = 0;

		virtual void shift(const Vector2d& shiftAmount) = 0;
		
		virtual void draw(Graphics graphics) const;

		//offset is rect2's offset to avoid rect1
		static Vector2d getCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static bool checkCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static bool checkCollision(CollisionRect* collisionRect, const RectangleD& filledRect);

	private:
		static Vector2d getFilledCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static Vector2d getPixelOnFilledCollisionOffset(CollisionRect* pixelRect, CollisionRect* filledRect);
		static Vector2d getPixelCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		
		static bool checkFilledCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);
		static bool checkPixelOnFilledCollision(CollisionRect* pixelRect, CollisionRect* filledRect);
		static bool checkPixelCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2);

		String tag;
	};
}

REGISTER_TYPE(fgl::CollisionRect)
