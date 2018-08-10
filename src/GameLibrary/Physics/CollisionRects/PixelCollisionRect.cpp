
#include <GameLibrary/Physics/CollisionRects/PixelCollisionRect.hpp>

namespace fgl
{
	PixelCollisionRect::PixelCollisionRect(const String& tag, const RectangleD& dstRect, const RectangleD& lastRect, const RectangleU srcRect, TextureImage* image, bool mirroredHorizontal, bool mirroredVertical)
		: CollisionRect(tag),
		dstRect(dstRect),
		lastRect(lastRect),
		srcRect(srcRect),
		boundingRect(dstRect),
		image(image),
		mirroredHorizontal(mirroredHorizontal),
		mirroredVertical(mirroredVertical),
		usesTransform(false) {
		//
	}

	PixelCollisionRect::PixelCollisionRect(const String& tag, const RectangleD& dstRect, const RectangleD& lastRect, const RectangleU srcRect, double rotation, const Vector2d& origin, TextureImage* image, bool mirroredHorizontal, bool mirroredVertical)
		: CollisionRect(tag),
		dstRect(dstRect),
		lastRect(lastRect),
		srcRect(srcRect),
		image(image),
		mirroredHorizontal(mirroredHorizontal),
		mirroredVertical(mirroredVertical),
		usesTransform(true) {
		srcTransform.rotate(rotation, origin);
		RectangleD relBoundingRect = srcTransform.transform(RectangleD(0, 0, dstRect.width, dstRect.height));
		boundingRect = RectangleD(dstRect.x+relBoundingRect.x, dstRect.y+relBoundingRect.y, relBoundingRect.width, relBoundingRect.height);
	}

	RectangleD PixelCollisionRect::getRect() const {
		return boundingRect;
	}
	
	RectangleD PixelCollisionRect::getPreviousRect() const {
		return lastRect;
	}

	bool PixelCollisionRect::isFilled() const {
		return false;
	}

	PixelIterator PixelCollisionRect::createPixelIterator(const RectangleD& loopRect, const Vector2d& increment) const {
		if(usesTransform) {
			return PixelIterator(image->getDimensions(), srcRect, boundingRect, loopRect, increment.x, increment.y, srcTransform, Vector2d(dstRect.width/srcRect.width, dstRect.height/srcRect.height), mirroredHorizontal, mirroredVertical);
		}
		else {
			return PixelIterator(image->getDimensions(), srcRect, boundingRect, loopRect, increment.x, increment.y, mirroredHorizontal, mirroredVertical);
		}
	}

	bool PixelCollisionRect::check(const PixelIterator& iterator) const {
		double index = iterator.getCurrentPixelIndex();
		if(index >= 0 && index < (double)image->getLength()) {
			return image->checkPixel((unsigned int)index);
		}
		return false;
	}
	
	bool PixelCollisionRect::check(const Vector2d& point) const {
		if(boundingRect.contains(point)) {
			if(usesTransform) {
				auto inverseTransform = srcTransform.getInverse();
				auto srcRectD = fgl::RectangleD((double)srcRect.x, (double)srcRect.y, (double)srcRect.width, (double)srcRect.height);
				Vector2d pixelPoint = inverseTransform.transform(point);
				pixelPoint.x *= srcRectD.width / dstRect.width;
				pixelPoint.y *= srcRectD.height / dstRect.height;
				if(mirroredHorizontal) {
					pixelPoint.x = srcRectD.x + (srcRectD.width - (pixelPoint.x-srcRectD.x));
				}
				else {
					pixelPoint.x = srcRectD.x + pixelPoint.x;
				}
				if(mirroredVertical) {
					pixelPoint.y = srcRectD.y + (srcRectD.height - (pixelPoint.y-srcRectD.y));
				}
				else {
					pixelPoint.y = srcRectD.y + pixelPoint.y;
				}
				if(pixelPoint.x >= srcRectD.getLeft() && pixelPoint.x < srcRectD.getRight() && pixelPoint.y >= srcRectD.getTop() && pixelPoint.y < srcRectD.getBottom()) {
					return image->checkPixel((unsigned int)pixelPoint.x, (unsigned int)pixelPoint.y);
				}
				return false;
			}
			return true;
		}
		return false;
	}

	Vector2d PixelCollisionRect::getPreferredIncrement() const {
		return Vector2d(dstRect.width/srcRect.width, dstRect.height/srcRect.height);
	}
}
