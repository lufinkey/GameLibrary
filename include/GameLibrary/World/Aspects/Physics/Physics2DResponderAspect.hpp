
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Physics/CollisionEventListener.hpp>

namespace fgl
{
	class Physics2DResponderAspect : public WorldObjectAspect, public CollisionEventListener
	{
	public:
		Physics2DResponderAspect();
		
		virtual void update(const ApplicationData& appData) override;
		
		void setDiminishAmount(const Vector2d& dimishAmount);
		const Vector2d& getDiminishAmount() const;
		
		void setDiminishStopAmount(const Vector2d& diminishStopAmount);
		const Vector2d& getDiminishStopAmount() const;
		
		void setBounceThreshold(const Vector2d& bounceThreshold);
		const Vector2d& getBounceThreshold() const;
		
		void setBounceRetentionAmount(const Vector2d& bounceRetentionAmount);
		const Vector2d& getBounceRetentionAmount() const;
		
	protected:
		virtual void onCollision(const CollisionEvent& event) override;
		virtual void onCollisionUpdate(const CollisionEvent& event) override;
		
	private:
		Vector2d prevVelocity;
		Vector2d diminishAmount;
		Vector2d diminishStopAmount;
		Vector2d bounceThreshold;
		Vector2d bounceRetentionAmount;
	};
}
