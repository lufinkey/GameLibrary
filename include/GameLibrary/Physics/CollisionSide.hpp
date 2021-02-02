
#pragma once

#include <GameLibrary/Utilities/String.hpp>

namespace fgl
{
	enum class CollisionSide {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	
	CollisionSide CollisionSide_getOpposite(CollisionSide side);
	String CollisionSide_toString(CollisionSide side);
}
