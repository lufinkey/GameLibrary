
#include <GameLibrary/Utilities/Direction/OctalDirection.hpp>

namespace fgl
{
	OctalDirection OctalDirection_fromVector2(const Vector2d& direction) {
    	if(direction.y < 0) {
    		if(direction.x == 0) {
    			return OctalDirection::UP;
    		}
    		else {
    			double ratLowLeft = .4142135624;
    			double ratHighLeft = 2.4142135624;
    			double ratLowRight = -ratLowLeft;
    			double ratHighRight = -ratHighLeft;
    			
    			double difRat =  direction.y / direction.x;
    			
    			if((difRat < ratLowRight) && (difRat > ratHighRight)) {
    				return OctalDirection::UP_RIGHT;
    			}
    			else if((difRat >= ratLowRight) && (difRat <= 0)) {
    				return OctalDirection::RIGHT;
    			}
    			else if((difRat <= ratLowLeft) && (difRat >= 0)) {
    				return OctalDirection::LEFT;
    			}
    			else if((difRat < ratHighLeft) && (difRat > ratLowLeft)) {
    				return OctalDirection::UP_LEFT;
    			}
    			else {
    				return OctalDirection::UP;
    			}
    		}
    	}
    	else if(direction.y > 0) {
    		if(direction.x == 0) {
    			return OctalDirection::DOWN;
    		}
    		else {
    			double ratLowRight = .4142135624;
    			double ratHighRight = 2.4142135624;
    			double ratLowLeft = -ratLowRight;
    			double ratHighLeft = -ratHighRight;
    			
    			double difRat =  direction.y / direction.x;
    			
    			if((difRat > ratHighLeft) && (difRat < ratLowLeft)) {
    				return OctalDirection::DOWN_LEFT;
    			}
    			else if((difRat >= ratLowLeft) && (difRat <= 0)) {
    				return OctalDirection::LEFT;
    			}
    			else if((difRat <= ratLowRight) && (difRat >= 0)) {
    				return OctalDirection::RIGHT;
    			}
    			else if((difRat > ratLowRight) && (difRat < ratHighRight)) {
    				return OctalDirection::DOWN_RIGHT;
    			}
    			else {
    				return OctalDirection::DOWN;
    			}
    		}
    	}
    	else {
    		if(direction.x < 0) {
    			return OctalDirection::LEFT;
    		}
    		else if(direction.x > 0) {
    			return OctalDirection::RIGHT;
    		}
    		else {
    			return OctalDirection::NONE;
    		}
    	}
	}
	
	
	
	Vector2d OctalDirection_toVector2(OctalDirection direction) {
		static double cosMult = fgl::Math::cos(fgl::Math::degtorad(45.0));
		switch(direction) {
			case OctalDirection::NONE:
				return { 0, 0 };
				
			case OctalDirection::UP:
				return { 0, -1 };
				
			case OctalDirection::UP_RIGHT:
				return { cosMult, -cosMult };
				
			case OctalDirection::RIGHT:
				return { 1, 0 };
				
			case OctalDirection::DOWN_RIGHT:
				return { cosMult, cosMult };
				
			case OctalDirection::DOWN:
				return { 0, 1 };
				
			case OctalDirection::DOWN_LEFT:
				return { -cosMult, cosMult };
				
			case OctalDirection::LEFT:
				return { -1, 0 };
				
			case OctalDirection::UP_LEFT:
				return { -cosMult, -cosMult };
				
			default:
				throw IllegalArgumentException("direction", "invalid OctalDirection enum");
		}
	}
	
	
	
	Vector2d OctalDirection_toFlatVector2(OctalDirection direction) {
		switch(direction) {
			case OctalDirection::NONE:
				return { 0, 0 };
				
			case OctalDirection::UP:
				return { 0, -1 };
				
			case OctalDirection::UP_RIGHT:
				return { 1, -1 };
				
			case OctalDirection::RIGHT:
				return { 1, 0 };
				
			case OctalDirection::DOWN_RIGHT:
				return { 1, 1 };
				
			case OctalDirection::DOWN:
				return { 0, 1 };
				
			case OctalDirection::DOWN_LEFT:
				return { -1, 1 };
				
			case OctalDirection::LEFT:
				return { -1, 0 };
				
			case OctalDirection::UP_LEFT:
				return { -1, -1 };
				
			default:
				throw IllegalArgumentException("direction", "invalid OctalDirection enum");
		}
	}
	
	
	String OctalDirection_toString(OctalDirection direction) {
		switch(direction) {
			case OctalDirection::UP:
				return "UP";
				
			case OctalDirection::UP_RIGHT:
				return "UP_RIGHT";
				
			case OctalDirection::RIGHT:
				return "RIGHT";
				
			case OctalDirection::DOWN_RIGHT:
				return "DOWN_RIGHT";
				
			case OctalDirection::DOWN:
				return "DOWN";
				
			case OctalDirection::DOWN_LEFT:
				return "DOWN_LEFT";
				
			case OctalDirection::LEFT:
				return "LEFT";
				
			case OctalDirection::UP_LEFT:
				return "UP_LEFT";
				
			case OctalDirection::NONE:
				return "NONE";
		}
		throw IllegalArgumentException("direction", "invalid OctalDirection enum");
	}
	
	OctalDirection OctalDirection_fromString(String str) {
		if(str == "UP") {
			return OctalDirection::UP;
		}
		else if(str == "UP_RIGHT") {
			return OctalDirection::UP_RIGHT;
		}
		else if(str == "RIGHT") {
			return OctalDirection::RIGHT;
		}
		else if(str == "DOWN_RIGHT") {
			return OctalDirection::DOWN_RIGHT;
		}
		else if(str == "DOWN") {
			return OctalDirection::DOWN;
		}
		else if(str == "DOWN_LEFT") {
			return OctalDirection::DOWN_LEFT;
		}
		else if(str == "LEFT") {
			return OctalDirection::LEFT;
		}
		else if(str == "UP_LEFT") {
			return OctalDirection::UP_LEFT;
		}
		else if(str == "NONE") {
			return OctalDirection::NONE;
		}
		throw IllegalArgumentException("str", "invalid OctalDirection string");
	}
}
