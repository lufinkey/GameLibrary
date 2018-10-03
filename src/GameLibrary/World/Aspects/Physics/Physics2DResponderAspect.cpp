
#include <GameLibrary/World/Aspects/Physics/Physics2DResponderAspect.hpp>
#include <GameLibrary/World/Aspects/Physics/Collidable2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Velocity2DAspect.hpp>
#include <GameLibrary/IO/Console.hpp>

namespace fgl
{
	Physics2DResponderAspect::Physics2DResponderAspect()
		: prevVelocity(0, 0),
		diminishAmount(0, 0),
		diminishStopAmount(0, 0),
		bounceThreshold(
			std::numeric_limits<double>::infinity(),
			std::numeric_limits<double>::infinity()),
		bounceRetentionAmount(0.6, 0.6) {
		//
	}
	
	void Physics2DResponderAspect::update(const ApplicationData& appData) {
		auto velocity2d = getAspect<Velocity2DAspect>();
		auto velocity = velocity2d->getVelocity();
		
		auto collidable = getAspect<Collidable2DAspect>();
		if(velocity.x == prevVelocity.x && !collidable->hasCollision(CollisionSide::LEFT) && !collidable->hasCollision(CollisionSide::RIGHT)) {
			velocity.x -= velocity.x * diminishAmount.x * appData.getFrameSpeedMultiplier();
			if(Math::abs(velocity.x) < diminishStopAmount.x) {
				velocity.x = 0;
			}
		}
		if(velocity.y == prevVelocity.y && !collidable->hasCollision(CollisionSide::TOP) && !collidable->hasCollision(CollisionSide::BOTTOM)) {
			velocity.y -= velocity.y * diminishAmount.y * appData.getFrameSpeedMultiplier();
			if(Math::abs(velocity.y) < diminishStopAmount.y) {
				velocity.y = 0;
			}
		}
		
		velocity2d->setVelocity(velocity);
		prevVelocity = velocity;
	}
	
	void Physics2DResponderAspect::setDiminishAmount(const Vector2d& diminishAmount_arg) {
		diminishAmount = diminishAmount_arg;
	}
	
	const Vector2d& Physics2DResponderAspect::getDiminishAmount() const {
		return diminishAmount;
	}
	
	void Physics2DResponderAspect::setDiminishStopAmount(const Vector2d& diminishStopAmount_arg) {
		diminishStopAmount = diminishStopAmount_arg;
	}
	
	const Vector2d& Physics2DResponderAspect::getDiminishStopAmount() const {
		return diminishStopAmount;
	}
	
	void Physics2DResponderAspect::setBounceThreshold(const Vector2d& bounceThreshold_arg) {
		bounceThreshold = bounceThreshold_arg;
	}
	
	const Vector2d& Physics2DResponderAspect::getBounceThreshold() const {
		return bounceThreshold;
	}
	
	void Physics2DResponderAspect::setBounceRetentionAmount(const Vector2d& bounceRetentionAmount_arg) {
		bounceRetentionAmount = bounceRetentionAmount_arg;
	}
	
	const Vector2d& Physics2DResponderAspect::getBounceRetentionAmount() const {
		return bounceRetentionAmount;
	}
	
	void Physics2DResponderAspect::onCollision(const CollisionEvent& event) {
		handleCollisionResponse(event);
	}
	
	void Physics2DResponderAspect::onCollisionUpdate(const CollisionEvent& event) {
		handleCollisionResponse(event);
	}
	
	#define CORRECT_VELOCITY_OVERFLOW(originalVel, finalVel, lowerSide, upperSide) \
		if((Math::sign(finalVel) == Math::sign(originalVel) || originalVel == 0 || finalVel == 0) \
			&& Math::abs(finalVel) > Math::abs(originalVel) \
			&& ((finalVel < 0 && side == lowerSide) || (finalVel > 0 && side == upperSide))) { \
			finalVel = originalVel; \
		}
	
	void Physics2DResponderAspect::handleCollisionResponse(const CollisionEvent& event) {
		auto target = event.getTarget();
		auto collided = event.getCollided();
		
		auto side = event.getCollisionSide();
		auto velocity2d = getAspect<Velocity2DAspect>();
		auto velocity = velocity2d->getVelocity();
		auto collidedVelocity = collided->getDisplacement() / event.getAppData()->getFrameSpeedMultiplier();
		auto mass = target->getMass();
		auto collidedMass = collided->getMass();
		
		// calculate final velocity
		auto finalVelocity = Vector2d(0,0);
		auto finalBounceVelocity = Vector2d(0,0);
		if(collided->isStaticCollisionBody()) {
			finalVelocity = collidedVelocity;
			CORRECT_VELOCITY_OVERFLOW(velocity.x, finalVelocity.x, CollisionSide::LEFT, CollisionSide::RIGHT)
			CORRECT_VELOCITY_OVERFLOW(velocity.y, finalVelocity.y, CollisionSide::TOP, CollisionSide::BOTTOM)
			finalBounceVelocity = (collidedVelocity - velocity) * Vector2d(bounceRetentionAmount.x, bounceRetentionAmount.y);
			CORRECT_VELOCITY_OVERFLOW(velocity.x, finalBounceVelocity.x, CollisionSide::LEFT, CollisionSide::RIGHT)
			CORRECT_VELOCITY_OVERFLOW(velocity.y, finalBounceVelocity.y, CollisionSide::TOP, CollisionSide::BOTTOM)
		}
		else {
			finalVelocity = ((velocity*mass) + (collidedVelocity*collidedMass))/(mass + collidedMass);
			CORRECT_VELOCITY_OVERFLOW(velocity.x, finalVelocity.x, CollisionSide::LEFT, CollisionSide::RIGHT)
			CORRECT_VELOCITY_OVERFLOW(velocity.y, finalVelocity.y, CollisionSide::TOP, CollisionSide::BOTTOM)
			finalBounceVelocity = finalVelocity * Vector2d(bounceRetentionAmount.x, bounceRetentionAmount.y);
			CORRECT_VELOCITY_OVERFLOW(velocity.x, finalBounceVelocity.x, CollisionSide::LEFT, CollisionSide::RIGHT)
			CORRECT_VELOCITY_OVERFLOW(velocity.y, finalBounceVelocity.y, CollisionSide::TOP, CollisionSide::BOTTOM)
		}
		// apply final velocity
		switch(event.getCollisionSide()) {
			case CollisionSide::TOP: // -y
				if(velocity.y <= -bounceThreshold.y) {
					velocity.y = finalBounceVelocity.y;
				}
				else {
					velocity.y = finalVelocity.y;
				}
				break;
				
			case CollisionSide::BOTTOM: // +y
				if(velocity.y >= bounceThreshold.y) {
					velocity.y = finalBounceVelocity.y;
				}
				else {
					velocity.y = finalVelocity.y;
				}
				break;
				
			case CollisionSide::LEFT: // -x
				if(velocity.x <= -bounceThreshold.x) {
					velocity.x = finalBounceVelocity.x;
				}
				else {
					velocity.x = finalVelocity.x;
				}
				break;
				
			case CollisionSide::RIGHT: // +x
				if(velocity.x >= bounceThreshold.x) {
					velocity.x = finalBounceVelocity.x;
				}
				else {
					velocity.x = finalVelocity.x;
				}
				break;
		}
		
		velocity2d->setVelocity(velocity);
	}
}
