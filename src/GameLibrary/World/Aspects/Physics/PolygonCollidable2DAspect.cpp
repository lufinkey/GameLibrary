
#include <GameLibrary/World/Aspects/Physics/PolygonCollidable2DAspect.hpp>
#include <GameLibrary/Physics/CollisionRects/PolygonCollisionRect.hpp>

namespace fgl
{
	PolygonCollidable2DAspect::PolygonCollidable2DAspect(PolygonD polygon, double mass)
		: Collidable2DAspect(mass),
		polygons({ { "0", polygon } }) {
		//
	}
	
	PolygonCollidable2DAspect::PolygonCollidable2DAspect(ArrayList<PolygonD> polygons, double mass)
		: Collidable2DAspect(mass),
		polygons(polygons.map<std::pair<String,PolygonD>>([](auto polygon, size_t index) -> auto {
			return std::make_pair((String)""+index, polygon);
		})) {
		//
	}
	
	PolygonCollidable2DAspect::PolygonCollidable2DAspect(BasicDictionary<String,PolygonD> polygons, double mass)
		: Collidable2DAspect(mass),
		polygons(polygons.begin(), polygons.end()) {
		//
	}
	
	ArrayList<const CollisionRect*> PolygonCollidable2DAspect::createCollisionRects() const {
		auto prevCollisionRects = getCollisionRects();
		ArrayList<const CollisionRect*> rects;
		rects.reserve(polygons.size());
		for(auto& pair : polygons) {
			auto& tag = pair.first;
			auto& polygon = pair.second;
			auto prevRect = prevCollisionRects.firstWhere([&](auto& collisionRect) -> bool {
				if(tag == collisionRect->getTag()) {
					return true;
				}
				return false;
			}, nullptr);
			auto collisionRect = new PolygonCollisionRect(tag, polygon, (prevRect ? prevRect->getRect() : polygon.getRectangle()));
			rects.add(collisionRect);
		}
		return rects;
	}
}
