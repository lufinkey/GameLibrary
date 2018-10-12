
#include <GameLibrary/Utilities/Direction/QuadDirection.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	String QuadDirection_toString(QuadDirection quadDirection) {
		switch(quadDirection) {
			case QuadDirection::NONE:
				return "NONE";
				
			case QuadDirection::UP:
				return "UP";
				
			case QuadDirection::DOWN:
				return "DOWN";
				
			case QuadDirection::LEFT:
				return "LEFT";
				
			case QuadDirection::RIGHT:
				return "RIGHT";
				
			default:
				throw IllegalArgumentException("quadDirection", "not a valid enum value");
		}
	}
	
	
	
	QuadDirection QuadDirection_fromVector2(Vector2d direction) {
		if(direction.x == 0 && direction.y == 0) {
			return QuadDirection::NONE;
		}
		double degrees = Math::normalizeDegrees(Math::radtodeg(Math::atan2(-direction.y, direction.x)));
		if(degrees <= 45 || degrees >= 315) {
        	return QuadDirection::RIGHT;
    	}
    	else if(degrees > 45 && degrees < 135) {
        	return QuadDirection::UP;
    	}
    	else if(degrees >= 135 && degrees <= 225) {
        	return QuadDirection::LEFT;
    	}
    	else if(degrees > 225 && degrees < 315) {
        	return QuadDirection::DOWN;
    	}
		throw IllegalStateException("something went wrong");
	}
}
