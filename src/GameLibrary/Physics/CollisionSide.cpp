
#include <GameLibrary/Physics/CollisionSide.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Utilities/String.hpp>

namespace fgl
{
	CollisionSide CollisionSide_getOpposite(CollisionSide side) {
		switch(side) {
			case CollisionSide::LEFT:
				return CollisionSide::RIGHT;
				
			case CollisionSide::RIGHT:
				return CollisionSide::LEFT;
				
			case CollisionSide::TOP:
				return CollisionSide::BOTTOM;
				
			case CollisionSide::BOTTOM:
				return CollisionSide::TOP;
		}
		throw IllegalArgumentException("side", "invalid CollisionSide enum value");
	}
	
	String CollisionSide_toString(CollisionSide side) {
		switch(side) {
			case CollisionSide::LEFT:
				return "LEFT";
				
			case CollisionSide::RIGHT:
				return "RIGHT";
				
			case CollisionSide::TOP:
				return "TOP";
				
			case CollisionSide::BOTTOM:
				return "BOTTOM";
		}
		throw IllegalArgumentException("side", "invalid CollisionSide enum value");
	}
}
