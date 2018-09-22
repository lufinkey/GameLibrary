
#pragma once

#include "Collidable2DAspect.hpp"

namespace fgl
{
	class PolygonCollidable2DAspect : public Collidable2DAspect
	{
	public:
		PolygonCollidable2DAspect(PolygonD polygon, double mass=1.0);
		PolygonCollidable2DAspect(ArrayList<PolygonD> polygons, double mass=1.0);
		PolygonCollidable2DAspect(BasicDictionary<String,PolygonD> polygons, double mass=1.0);
		
		void setPolygons(const BasicDictionary<String,PolygonD>& boxes);
		void setPolygons(const ArrayList<PolygonD>& boxes);
		const BasicDictionary<String,PolygonD>& getPolygons() const;
		
		void setPolygon(String tag, PolygonD polygon);
		const PolygonD& getPolygon(const String& tag) const;
		
	protected:
		virtual ArrayList<const CollisionRect*> createCollisionRects() const override;

	private:
		BasicDictionary<String,PolygonD> polygons;
	};
}
