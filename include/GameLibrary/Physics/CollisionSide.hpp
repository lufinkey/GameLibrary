
#pragma once

namespace fgl
{
	enum class CollisionSide
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	
	CollisionSide CollisionSide_getOpposite(CollisionSide side);
}
