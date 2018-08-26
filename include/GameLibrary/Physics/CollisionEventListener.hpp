
#pragma once

#include "CollisionEvent.hpp"

namespace fgl
{
	class CollisionEventListener
	{
		friend class CollisionManager;
	public:
		virtual ~CollisionEventListener() = default;
		
	protected:
		virtual void onCollision(const CollisionEvent& event) {}
		virtual void onCollisionUpdate(const CollisionEvent& event) {}
		virtual void onCollisionFinish(const CollisionEvent& event) {}
	};
}
