
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
		
	protected:
		virtual ArrayList<const CollisionRect*> createCollisionRects() const override;

	private:
		ArrayList<std::pair<String,PolygonD>> polygons;
	};
}
