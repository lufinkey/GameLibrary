
#include <GameLibrary/World/Aspects/Physics/Physics2DResponderAspect.hpp>
#include <GameLibrary/World/Aspects/Physics/Collidable2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Velocity2DAspect.hpp>

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
		
		if(velocity.x == prevVelocity.x) {
			velocity.x -= velocity.x * diminishAmount.x * appData.getFrameSpeedMultiplier();
			if(Math::abs(velocity.x) < diminishStopAmount.x) {
				velocity.x = 0;
			}
		}
		if(velocity.y == prevVelocity.y) {
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
	
	void Physics2DResponderAspect::onAddToObject(WorldObject* object) {
		auto collidable = object->getAspect<Collidable2DAspect>();
		if(collidable != nullptr) {
			collidable->addCollisionEventListener(this);
		}
	}
	
	void Physics2DResponderAspect::onCollision(const CollisionEvent& event) {
		auto velocity2d = getAspect<Velocity2DAspect>();
		auto velocity = velocity2d->getVelocity();
		
		auto frameSpeedMult = event.getAppData()->getFrameSpeedMultiplier();
		auto gravityStop = velocity2d->getGravityIncrement(frameSpeedMult * 3.0);
		
		// handle bounce
		switch(event.getCollisionSide()) {
			case CollisionSide::TOP: // -y
				if(velocity.y < 0) {
					if(Math::abs(velocity.y) <= Math::abs(gravityStop.y)) {
						velocity.y = 0;
					}
					else if(velocity.y <= -bounceThreshold.y) {
						velocity.y = -velocity.y * bounceRetentionAmount.y;
					}
					else {
						velocity.y = 0;
					}
				}
				break;
				
			case CollisionSide::BOTTOM: // +y
				if(velocity.y > 0) {
					if(Math::abs(velocity.y) <= Math::abs(gravityStop.y)) {
						velocity.y = 0;
					}
					else if(velocity.y >= bounceThreshold.y) {
						velocity.y = -velocity.y * bounceRetentionAmount.y;
					}
					else {
						velocity.y = 0;
					}
				}
				break;
				
			case CollisionSide::LEFT: // -x
				if(velocity.x < 0) {
					if(Math::abs(velocity.x) <= Math::abs(gravityStop.x)) {
						velocity.x = 0;
					}
					else if(velocity.x <= -bounceThreshold.x) {
						velocity.x = -velocity.x * bounceRetentionAmount.x;
					}
					else {
						velocity.x = 0;
					}
				}
				break;
				
			case CollisionSide::RIGHT: // +x
				if(velocity.x > 0) {
					if(Math::abs(velocity.x) <= Math::abs(gravityStop.x)) {
						velocity.x = 0;
					}
					else if(velocity.x >= bounceThreshold.x) {
						velocity.x = -velocity.x * bounceRetentionAmount.x;
					}
					else {
						velocity.x = 0;
					}
				}
				break;
		}
		
		velocity2d->setVelocity(velocity);
	}
	
	void Physics2DResponderAspect::onCollisionUpdate(const CollisionEvent& event) {
		auto velocity2d = getAspect<Velocity2DAspect>();
		auto velocity = velocity2d->getVelocity();
		
		// stop velocity
		switch(event.getCollisionSide()) {
			case CollisionSide::TOP: // -y
				if(velocity.y < 0) {
					velocity.y = 0;
				}
				break;
				
			case CollisionSide::BOTTOM: // +y
				if(velocity.y > 0) {
					velocity.y = 0;
				}
				break;
				
			case CollisionSide::LEFT: // -x
				if(velocity.x < 0) {
					velocity.x = 0;
				}
				break;
				
			case CollisionSide::RIGHT: // +x
				if(velocity.x > 0) {
					velocity.x = 0;
				}
				break;
		}
		
		velocity2d->setVelocity(velocity);
	}
}
