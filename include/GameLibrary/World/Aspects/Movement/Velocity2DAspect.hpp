
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>

namespace fgl
{
	class Velocity2DAspect : public WorldObjectAspect
	{
	public:
		explicit Velocity2DAspect(const Vector2d& velocity=Vector2d(0,0), double gravityAmount=1.0, bool gravityEnabled = true);
		
		virtual void update(const ApplicationData& appData) override;
		
		void setVelocity(const Vector2d& velocity);
		const Vector2d& getVelocity() const;
		
		void setGravityAmount(double amount);
		double getGravityAmount() const;
		
		void setGravityEnabled(bool gravityEnabled);
		bool isGravityEnabled() const;
		
		Vector2d getGravityIncrement(double frameSpeedMultiplier) const;
		
		void applyForce(const Vector2d& force);
		
	private:
		Vector2d velocity;
		double gravityAmount;
		bool gravityEnabled;
	};
}
