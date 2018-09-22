
#pragma once

#include "Collidable2DAspect.hpp"

namespace fgl
{
	class BoxCollidable2DAspect : public Collidable2DAspect
	{
	public:
		BoxCollidable2DAspect(RectangleD box, double mass=1.0);
		BoxCollidable2DAspect(ArrayList<RectangleD> boxes, double mass=1.0);
		BoxCollidable2DAspect(BasicDictionary<String,RectangleD> boxes, double mass=1.0);
		
		void setBoxes(const BasicDictionary<String,RectangleD>& boxes);
		void setBoxes(const ArrayList<RectangleD>& boxes);
		const BasicDictionary<String,RectangleD>& getBoxes() const;
		
		void setBox(String tag, const RectangleD& rect);
		const RectangleD& getBox(const String& tag) const;
		
	protected:
		virtual ArrayList<const CollisionRect*> createCollisionRects() const override;

	private:
		BasicDictionary<String,RectangleD> boxes;
	};
}
