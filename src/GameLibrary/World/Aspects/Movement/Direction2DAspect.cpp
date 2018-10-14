
#include <GameLibrary/World/Aspects/Movement/Direction2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform3DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Velocity2DAspect.hpp>
#include <GameLibrary/World/Aspects/Physics/Collidable2DAspect.hpp>

namespace fgl
{
	Direction2DAspect::Direction2DAspect(const Vector2d& direction, double speed)
		: direction(direction),
		speed(speed) {
		//
	}
	
	void Direction2DAspect::update(const ApplicationData& appData) {
		auto moveSpeed = direction * speed * getSpeedModifier();
		
		if(speedApplyer) {
			speedApplyer(appData, moveSpeed, prevSpeed);
		}
		else {
			auto translation = moveSpeed * appData.getFrameSpeedMultiplier();
			// try to use transform3d
			auto transform3d = getAspect<Transform3DAspect>();
			if(transform3d != nullptr) {
				auto position = transform3d->getPosition();
				position.x += translation.x;
				position.y += translation.y;
				transform3d->setPosition(position);
				return;
			}
			// try to use transform2d
			auto transform2d = getAspect<Transform2DAspect>();
			if(transform2d != nullptr) {
				auto position = transform2d->getPosition();
				position += translation;
				transform2d->setPosition(position);
				return;
			}
		}
		
		prevSpeed = moveSpeed;
	}
	
	void Direction2DAspect::setDirection(const Vector2d& direction_arg) {
		if(direction != direction_arg) {
			direction = direction_arg;
			onDirectionChange(direction);
		}
	}
	
	const Vector2d& Direction2DAspect::getDirection() const {
		return direction;
	}
	
	void Direction2DAspect::setSpeed(double speed_arg) {
		speed = speed_arg;
	}
	
	double Direction2DAspect::getSpeed() const {
		return speed;
	}
	
	void Direction2DAspect::setSpeedApplyer(SpeedApplyerFunc speedApplyer_arg) {
		speedApplyer = speedApplyer_arg;
	}
	
	const SpeedApplyerFunc& Direction2DAspect::getSpeedApplyer() const {
		return speedApplyer;
	}
	
	void Direction2DAspect::addListener(Direction2DListener* listener) {
		listeners.push_back(listener);
	}
	
	void Direction2DAspect::removeListener(Direction2DListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	void Direction2DAspect::addDelegate(Direction2DDelegate* delegate) {
		delegates.push_back(delegate);
	}
	
	void Direction2DAspect::removeDelegate(Direction2DDelegate* delegate) {
		auto delegateIt = std::find(delegates.begin(), delegates.end(), delegate);
		if(delegateIt != delegates.end()) {
			delegates.erase(delegateIt);
		}
	}
	
	void Direction2DAspect::onDirectionChange(Vector2d direction) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onDirectionChange(this, direction);
			}
		}
	}
	
	Vector2d Direction2DAspect::getSpeedModifier() const {
		auto modifier = Vector2d{ 1.0, 1.0 };
		for(auto delegate : delegates) {
			modifier *= delegate->getSpeedModifier(this);
		}
		return modifier;
	}
	
	
	
	SpeedApplyerFunc Direction2DAspect::createVelocity2DXSpeedApplyer(WorldObject* object, SpeedApplyerOptions options) {
		return SpeedApplyerFunc([=](auto& appData, auto speed, auto prevSpeed) {
			auto direction2d = object->getAspect<Direction2DAspect>();
			auto velocity2d = object->getAspect<Velocity2DAspect>();
			auto collidable = object->getAspect<Collidable2DAspect>();
			
			auto velocity = velocity2d->getVelocity();
			auto dirSpeed = direction2d->getSpeed();
			bool isOnGround = collidable ? (collidable->getCollidedCountOnSide(CollisionSide::BOTTOM) > 0) : false;
			auto possibleAccel = speed - velocity;
			
			double maxSpeedChange = isOnGround ? options.maxSlowDownChange : options.maxAirSlowDownChange;
			if(Math::sign(velocity.x) == Math::sign(speed.x)) {
				if(Math::abs(velocity.x) < Math::abs(speed.x)) {
					maxSpeedChange = isOnGround ? options.maxSpeedUpChange : options.maxAirSpeedUpChange;
				}
				else if(dirSpeed < Math::abs(velocity.x)) {
					maxSpeedChange = 0;
				}
			}
			if(maxSpeedChange != 0) {
				auto xMoveRatio = Math::abs(possibleAccel.x / (maxSpeedChange * appData.getFrameSpeedMultiplier()));
				if(xMoveRatio <= 1.0) {
					velocity.x = speed.x;
				}
				else {
					auto xMove = possibleAccel.x / xMoveRatio;
					velocity.x += xMove;
				}
				velocity2d->setVelocity(velocity);
			}
		});
	}
	
	
	
	void Direction2DListener::onDirectionChange(Direction2DAspect* aspect, Vector2d direction) {
		// open for implementation
	}
}
