
#include <GameLibrary/Physics/CollisionRectBuilder.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PixelCollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/PolygonCollisionRect.hpp>

namespace fgl
{
	const CollisionRect* CollisionRectBuilder::fromRect(const String& tag, const RectangleD& rect, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& resolution) {
		return fromRect(tag, rect, prevRects, 0, resolution);
	}
	
	const CollisionRect* CollisionRectBuilder::fromRect(const String& tag, const RectangleD& rect, const ArrayList<const CollisionRect*>& prevRects, double rotation, const Vector2d& resolution) {
		auto lastRect = getMatchingRect(prevRects, tag, rect);
		if(rotation != 0.0) {
			auto transform = TransformD();
			transform.rotate(rotation);
			auto polygon = transform.transform(rect.toPolygon());
			return new PolygonCollisionRect(tag, polygon, lastRect, resolution);
		}
		return new BoxCollisionRect(tag, rect, lastRect, resolution);
	}
	
	const CollisionRect* CollisionRectBuilder::fromAnimation(const String& tag, Animation* animation, size_t frameIndex, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& origin, bool mirroredHorizontal, bool mirroredVertical) {
		auto& frame = animation->getFrame(frameIndex);
		auto animSize = (fgl::Vector2d)frame.getSourceRect().getSize();
		return fromAnimation(tag, animation, frameIndex, prevRects, origin, animSize, 0.0, mirroredHorizontal, mirroredVertical);
	}
	
	const CollisionRect* CollisionRectBuilder::fromAnimation(const String& tag, Animation* animation, size_t frameIndex, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& origin, const Vector2d& size, double rotation, bool mirroredHorizontal, bool mirroredVertical) {
		if(animation == nullptr) {
			return {};
		}
		auto& frame = animation->getFrame(frameIndex);
		auto img = frame.getImage();
		auto srcRect = frame.getSourceRect();
		auto rect = RectangleD(-origin.x, -origin.y, size.x, size.y);
		auto lastRect = getMatchingRect(prevRects, tag, rect);
		if(rotation != 0.0) {
			return new PixelCollisionRect(tag, rect, lastRect, srcRect, rotation, origin, img, mirroredHorizontal, mirroredVertical);
		}
		return new PixelCollisionRect(tag, rect, lastRect, srcRect, img, mirroredHorizontal, mirroredVertical);
	}
	
	const CollisionRect* CollisionRectBuilder::fromPolygon(const String& tag, const PolygonD& polygon, const ArrayList<const CollisionRect*>& prevRects, const Vector2d& resolution) {
		auto rect = polygon.getRectangle();
		auto lastRect = getMatchingRect(prevRects, tag, rect);
		if(rect.toPolygon() == polygon) {
			// polygon is a rectangle, just make a box collidable
			return new BoxCollisionRect(tag, rect, lastRect, resolution);
		}
		return new PolygonCollisionRect(tag, polygon, lastRect, resolution);
	}
	
	size_t CollisionRectBuilder::findMatchingRectIndex(const ArrayList<const CollisionRect*>& collisionRects, const String& tag) {
		return collisionRects.indexWhere([tag](auto& rect) -> bool {
			if(rect->getTag()==tag) {
				return true;
			}
			return false;
		});
	}
	
	RectangleD CollisionRectBuilder::getMatchingRect(const ArrayList<const CollisionRect*>& collisionRects, const String& tag, const RectangleD& currentRect) {
		size_t matchingRectIndex = findMatchingRectIndex(collisionRects, tag);
		if(matchingRectIndex==(size_t)-1) {
			return currentRect;
		}
		return collisionRects[matchingRectIndex]->getRect();
	}
}
