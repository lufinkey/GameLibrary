
#pragma once

#include "Collidable2DAspect.hpp"

namespace fgl
{
	class BoxCollidable2DAspect : public Collidable2DAspect
	{
	public:
		BoxCollidable2DAspect(const ArrayList<RectangleD>& boxes, double mass=1.0);
		BoxCollidable2DAspect(const BasicDictionary<String,RectangleD>& boxes, double mass=1.0);
		
	protected:
		virtual ArrayList<const CollisionRect*> createCollisionRects() const override;

	private:
		ArrayList<std::pair<String,RectangleD>> boxes;
	};
}
