
#include <GameLibrary/World/Aspects/Physics/PolygonCollidable2DAspect.hpp>
#include <GameLibrary/Physics/CollisionRects/PolygonCollisionRect.hpp>

namespace fgl
{
	PolygonCollidable2DAspect::PolygonCollidable2DAspect(PolygonD polygon, double mass)
		: Collidable2DAspect(mass),
		polygons({ { "0", polygon } }) {
		//
	}
	
	PolygonCollidable2DAspect::PolygonCollidable2DAspect(BasicDictionary<String,PolygonD> polygons, double mass)
		: Collidable2DAspect(mass),
		polygons(polygons) {
		//
	}
	
	
	
	void PolygonCollidable2DAspect::setPolygons(const BasicDictionary<String,PolygonD>& polygons_arg) {
		polygons = polygons_arg;
		setNeedsNewCollisionRects();
	}
	
	void PolygonCollidable2DAspect::setPolygons(const ArrayList<PolygonD>& polygons_arg) {
		polygons.clear();
		for(size_t i=0; i<polygons_arg.size(); i++) {
			polygons[(String)""+i] = polygons_arg[i];
		}
		setNeedsNewCollisionRects();
	}
	
	const BasicDictionary<String,PolygonD>& PolygonCollidable2DAspect::getPolygons() const {
		return polygons;
	}
	
	
	
	void PolygonCollidable2DAspect::setPolygon(String tag, PolygonD polygon) {
		polygons.set(tag, polygon);
		setNeedsNewCollisionRects();
	}
	
	const PolygonD& PolygonCollidable2DAspect::getPolygon(const String& tag) const {
		return polygons.get(tag);
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
