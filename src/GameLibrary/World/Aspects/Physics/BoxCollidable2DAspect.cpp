
#include <GameLibrary/World/Aspects/Physics/BoxCollidable2DAspect.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>

namespace fgl
{
	BoxCollidable2DAspect::BoxCollidable2DAspect(RectangleD box, double mass)
		: Collidable2DAspect(mass),
		boxes({ {"0", box} }) {
		//
	}
	
	BoxCollidable2DAspect::BoxCollidable2DAspect(BasicDictionary<String,RectangleD> boxes, double mass)
		: Collidable2DAspect(mass),
		boxes(boxes) {
		//
	}
	
	
	
	void BoxCollidable2DAspect::setBoxes(const BasicDictionary<String,RectangleD>& boxes_arg) {
		boxes = boxes_arg;
		setNeedsNewCollisionRects();
	}
	
	void BoxCollidable2DAspect::setBoxes(const ArrayList<RectangleD>& boxes_arg) {
		boxes.clear();
		for(size_t i=0; i<boxes_arg.size(); i++) {
			boxes[(String)""+i] = boxes_arg[i];
		}
		setNeedsNewCollisionRects();
	}
	
	const BasicDictionary<String,RectangleD>& BoxCollidable2DAspect::getBoxes() const {
		return boxes;
	}
	
	
	
	void BoxCollidable2DAspect::setBox(String tag, const RectangleD& rect) {
		boxes.set(tag, rect);
		setNeedsNewCollisionRects();
	}
	
	const RectangleD& BoxCollidable2DAspect::getBox(const String& tag) const {
		return boxes.get(tag);
	}
	
	
	
	ArrayList<const CollisionRect*> BoxCollidable2DAspect::createCollisionRects() const {
		auto prevCollisionRects = getCollisionRects();
		ArrayList<const CollisionRect*> rects;
		rects.reserve(boxes.size());
		for(auto& pair : boxes) {
			auto& tag = pair.first;
			auto& box = pair.second;
			auto prevRect = prevCollisionRects.firstWhere([&](auto& collisionRect) -> bool {
				if(tag == collisionRect->getTag()) {
					return true;
				}
				return false;
			}, nullptr);
			auto collisionRect = new BoxCollisionRect(tag, box, (prevRect ? prevRect->getRect() : box));
			rects.add(collisionRect);
		}
		return rects;
	}
}
