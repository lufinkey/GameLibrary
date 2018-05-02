
#include <GameLibrary/Physics/CollisionSide.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Utilities/String.hpp>

namespace fgl
{
	CollisionSide CollisionSide_getOpposite(CollisionSide side)
	{
		switch(side)
		{
			case COLLISIONSIDE_LEFT:
				return COLLISIONSIDE_RIGHT;
				
			case COLLISIONSIDE_RIGHT:
				return COLLISIONSIDE_LEFT;
				
			case COLLISIONSIDE_TOP:
				return COLLISIONSIDE_BOTTOM;
				
			case COLLISIONSIDE_BOTTOM:
				return COLLISIONSIDE_TOP;
		}
		throw IllegalArgumentException("side", "invalid CollisionSide enum value");
	}
}
