
#include <GameLibrary/World/Aspects/Physics/BoxCollidable2DAspect.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>

namespace fgl
{
	BoxCollidable2DAspect::BoxCollidable2DAspect(RectangleD box, double mass)
		: Collidable2DAspect(mass),
		boxes({ {"0", box} }) {
		//
	}
	
	BoxCollidable2DAspect::BoxCollidable2DAspect(ArrayList<RectangleD> boxes, double mass)
		: Collidable2DAspect(mass),
		boxes(boxes.map<std::pair<String,RectangleD>>([](auto box, size_t index) -> auto {
			return std::make_pair((String)""+index, box);
		})) {
		//
	}
	
	BoxCollidable2DAspect::BoxCollidable2DAspect(BasicDictionary<String,RectangleD> boxes, double mass)
		: Collidable2DAspect(mass),
		boxes(boxes.begin(), boxes.end()) {
		//
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
