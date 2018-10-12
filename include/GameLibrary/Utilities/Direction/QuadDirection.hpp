
#pragma once

#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>

namespace fgl
{
	enum class QuadDirection
	{
		NONE = 0,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	
	String QuadDirection_toString(QuadDirection quadDirection);
	QuadDirection QuadDirection_fromVector2(Vector2d direction);
}

