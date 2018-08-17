
#pragma once

#include <GameLibrary/Utilities/Geometry/Vector2.hpp>

namespace fgl
{
	struct TransformState
	{
		constexpr TransformState(Vector2d position, double rotation)
			: position(position),
			rotation(rotation) {
			//
		}
		
		constexpr bool equals(const TransformState& transformState) const {
			if(position == transformState.position && rotation == transformState.rotation) {
				return true;
			}
			return false;
		}
		
		constexpr bool operator==(const TransformState& transformState) const {
			return equals(transformState);
		}
		
		constexpr bool operator!=(const TransformState& transformState) const {
			return !equals(transformState);
		}
		
		Vector2d position;
		double rotation;
	};
}
