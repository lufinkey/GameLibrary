
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Physics/CollisionEventListener.hpp>

namespace fgl
{
	class Physics2DResponderAspect : public WorldObjectAspect, public CollisionEventListener
	{
	public:
		typedef std::function<bool(CollisionSide)> BounceCriteriaFunc;
		
		Physics2DResponderAspect();
		
		virtual void update(const ApplicationData& appData) override;
		
		void setDiminishAmount(const Vector2d& dimishAmount);
		const Vector2d& getDiminishAmount() const;
		
		void setDiminishStopAmount(const Vector2d& diminishStopAmount);
		const Vector2d& getDiminishStopAmount() const;
		
		void setBounceThresholds(BasicDictionary<CollisionSide,double> bounceThresholds);
		void setBounceThreshold(double threshold);
		void setBounceThreshold(CollisionSide side, double threshold);
		double getBounceThreshold(CollisionSide side) const;
		
		void setBounceRetentionAmounts(BasicDictionary<CollisionSide,double> bounceRetentionAmounts);
		void setBounceRetentionAmount(double retentionAmount);
		void setBounceRetentionAmount(CollisionSide side, double retentionAmount);
		double getBounceRetentionAmount(CollisionSide side) const;
		
		void setBounceCriteria(BounceCriteriaFunc criteriaFunc);
		const BounceCriteriaFunc& getBounceCriteria() const;
		
	protected:
		virtual void onCollision(const CollisionEvent& event) override;
		virtual void onCollisionUpdate(const CollisionEvent& event) override;
		
	private:
		void handleCollisionResponse(const CollisionEvent& event);
		
		Vector2d prevVelocity;
		Vector2d diminishAmount;
		Vector2d diminishStopAmount;
		BasicDictionary<CollisionSide,double> bounceThresholds;
		BasicDictionary<CollisionSide,double> bounceRetentionAmounts;
		BounceCriteriaFunc bounceCriteria;
	};
}
