
#pragma once

#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	enum class OctalDirection
	{
		NONE,
		UP,
		UP_RIGHT,
		RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT,
		LEFT,
		UP_LEFT
	};
	
	
	
	inline ArrayList<OctalDirection> nonZeroOctalDirections() {
		return {
			OctalDirection::UP,
			OctalDirection::UP_RIGHT,
			OctalDirection::RIGHT,
			OctalDirection::DOWN_RIGHT,
			OctalDirection::DOWN,
			OctalDirection::DOWN_LEFT,
			OctalDirection::LEFT,
			OctalDirection::UP_LEFT
		};
	}
	
	
	
	constexpr OctalDirection getOppositeOctalDirection(OctalDirection direction) {
		switch(direction) {
			case OctalDirection::NONE:
				return OctalDirection::NONE;
				
			case OctalDirection::UP:
				return OctalDirection::DOWN;
				
			case OctalDirection::UP_RIGHT:
				return OctalDirection::DOWN_LEFT;
				
			case OctalDirection::RIGHT:
				return OctalDirection::LEFT;
				
			case OctalDirection::DOWN_RIGHT:
				return OctalDirection::UP_LEFT;
				
			case OctalDirection::DOWN:
				return OctalDirection::UP;
				
			case OctalDirection::DOWN_LEFT:
				return OctalDirection::UP_RIGHT;
				
			case OctalDirection::LEFT:
				return OctalDirection::RIGHT;
				
			case OctalDirection::UP_LEFT:
				return OctalDirection::DOWN_RIGHT;
				
			default:
				throw IllegalArgumentException("direction", "invalid OctalDirection enum");
		}
	}
	
	
	
	constexpr std::tuple<OctalDirection, OctalDirection> getAdjacentOctalDirections(OctalDirection direction) {
		switch(direction) {
			case OctalDirection::NONE:
				return std::make_tuple( OctalDirection::NONE, OctalDirection::NONE );
				
			case OctalDirection::UP:
				return std::make_tuple( OctalDirection::UP_LEFT, OctalDirection::UP_RIGHT );
				
			case OctalDirection::UP_RIGHT:
				return std::make_tuple( OctalDirection::UP, OctalDirection::RIGHT );
				
			case OctalDirection::RIGHT:
				return std::make_tuple( OctalDirection::UP_RIGHT, OctalDirection::DOWN_RIGHT );
				
			case OctalDirection::DOWN_RIGHT:
				return std::make_tuple( OctalDirection::RIGHT, OctalDirection::DOWN );
				
			case OctalDirection::DOWN:
				return std::make_tuple( OctalDirection::DOWN_RIGHT, OctalDirection::DOWN_LEFT );
				
			case OctalDirection::DOWN_LEFT:
				return std::make_tuple( OctalDirection::DOWN, OctalDirection::LEFT );
				
			case OctalDirection::LEFT:
				return std::make_tuple( OctalDirection::DOWN_LEFT, OctalDirection::UP_LEFT );
				
			case OctalDirection::UP_LEFT:
				return std::make_tuple( OctalDirection::LEFT, OctalDirection::UP );
				
			default:
				throw fgl::IllegalArgumentException("direction", "invalid OctalDirection enum");
		}
	}
	
	
	
	constexpr Vector2d getOctalDirectionCornerPoint(OctalDirection direction, const RectangleD& rect) {
		switch(direction) {
			case OctalDirection::NONE:
				return rect.getCenter();
				
			case OctalDirection::UP:
				return rect.getTopCenter();
				
			case OctalDirection::UP_RIGHT:
				return rect.getTopRight();
				
			case OctalDirection::RIGHT:
				return rect.getRightCenter();
				
			case OctalDirection::DOWN_RIGHT:
				return rect.getBottomRight();
				
			case OctalDirection::DOWN:
				return rect.getBottomCenter();
				
			case OctalDirection::DOWN_LEFT:
				return rect.getBottomLeft();
				
			case OctalDirection::LEFT:
				return rect.getLeftCenter();
				
			case OctalDirection::UP_LEFT:
				return rect.getTopLeft();
				
			default:
				throw IllegalArgumentException("direction", "invalid OctalDirection enum");
		}
	}
	
	String OctalDirection_toString(OctalDirection direction);
	OctalDirection OctalDirection_fromString(String str);
	
	
	
	OctalDirection getOctalDirection(const Vector2d& direction);
	Vector2d getOctalDirectionVector(OctalDirection direction);
	Vector2d getFlatOctalDirectionVector(OctalDirection direction);
}
