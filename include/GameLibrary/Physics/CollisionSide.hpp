
#pragma once

namespace fgl
{
	typedef enum
	{
		COLLISIONSIDE_TOP,
		COLLISIONSIDE_BOTTOM,
		COLLISIONSIDE_LEFT,
		COLLISIONSIDE_RIGHT
	} CollisionSide;
	
	CollisionSide CollisionSide_getOpposite(CollisionSide side);
}
