
#pragma once

#include "CollisionRect.hpp"
#include <GameLibrary/Graphics/TextureImage.hpp>
#include <GameLibrary/Utilities/Geometry/Transform.hpp>

namespace fgl
{
	class PixelCollisionRect : public CollisionRect
	{
	public:
		PixelCollisionRect(const String& tag, const RectangleD& dstRect, const RectangleD& lastRect, const RectangleU srcRect, TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);
		PixelCollisionRect(const String& tag, const RectangleD& dstRect, const RectangleD& lastRect, const RectangleU srcRect, double rotation, const Vector2d& origin, TextureImage* image, bool mirroredHorizontal, bool mirroredVertical);

		virtual RectangleD getRect() const override;
		virtual RectangleD getPreviousRect() const override;
		virtual bool isFilled() const override;
		virtual PixelIterator createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const override;
		virtual bool check(const PixelIterator& iterator) const override;

		virtual Vector2d getPreferredIncrement() const override;

		virtual void shift(const Vector2d& shiftAmount) override;

	private:
		RectangleD dstRect;
		RectangleD lastRect;
		RectangleU srcRect;
		RectangleD boundingRect;
		TransformD srcTransform;
		TextureImage* image;

		bool mirroredHorizontal;
		bool mirroredVertical;

		bool usesTransform;
	};
}
