
#include <GameLibrary/World/Aspects/Movement/Velocity2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/World/World.hpp>

namespace fgl
{
	Velocity2DAspect::Velocity2DAspect(const Vector2d& velocity, double gravityAmount, bool gravityEnabled)
		: velocity(velocity),
		gravityAmount(gravityAmount),
		gravityEnabled(gravityEnabled) {
		//
	}
	
	void Velocity2DAspect::update(const ApplicationData& appData) {
		// update position
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d != nullptr) {
			auto position = transform2d->getPosition();
			position += velocity * appData.getFrameSpeedMultiplier();
			transform2d->setPosition(position);
		}
		// update gravity
		if(gravityEnabled && gravityAmount != 0.0 && (transform2d == nullptr || transform2d->getParent() == nullptr)) {
			velocity += getGravityIncrement(appData.getFrameSpeedMultiplier());
		}
	}
	
	void Velocity2DAspect::setVelocity(const Vector2d& velocity_arg) {
		velocity = velocity_arg;
	}
	
	const Vector2d& Velocity2DAspect::getVelocity() const {
		return velocity;
	}
	
	void Velocity2DAspect::setGravityAmount(double amount) {
		gravityAmount = amount;
	}
	
	double Velocity2DAspect::getGravityAmount() const {
		return gravityAmount;
	}
	
	void Velocity2DAspect::setGravityEnabled(bool gravityEnabled_arg) {
		gravityEnabled = gravityEnabled_arg;
	}
	
	bool Velocity2DAspect::isGravityEnabled() const {
		return gravityEnabled;
	}
	
	Vector2d Velocity2DAspect::getGravityIncrement(double frameSpeedMultiplier) const {
		auto world = getWorld();
		if(world != nullptr) {
			auto gravity = world->getGravity();
			return gravity * gravityAmount * frameSpeedMultiplier;
		}
		return {0, 0};
	}
	
	void Velocity2DAspect::applyForce(const Vector2d& force) {
		auto collidable = getAspect<Collidable>();
		double mass = 1.0;
		if(collidable != nullptr) {
			mass = collidable->getMass();
		}
		velocity += force / mass;
	}
}
