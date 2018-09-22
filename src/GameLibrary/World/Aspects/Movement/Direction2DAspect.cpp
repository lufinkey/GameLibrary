
#include <GameLibrary/World/Aspects/Movement/Direction2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform3DAspect.hpp>

namespace fgl
{
	Direction2DAspect::Direction2DAspect(const Vector2d& direction, double speed, bool listenerEvents)
		: direction(direction),
		speed(speed) {
		//
	}
	
	void Direction2DAspect::update(const ApplicationData& appData) {
		auto transform3d = getAspect<Transform3DAspect>();
		if(transform3d != nullptr) {
			auto position = transform3d->getPosition();
			position.x += direction.x * speed * appData.getFrameSpeedMultiplier();
			position.y += direction.y * speed * appData.getFrameSpeedMultiplier();
			transform3d->setPosition(position);
		}
		else {
			auto transform2d = getAspect<Transform2DAspect>();
			if(transform2d != nullptr) {
				auto position = transform2d->getPosition();
				position.x += direction.x * speed * appData.getFrameSpeedMultiplier();
				position.y += direction.y * speed * appData.getFrameSpeedMultiplier();
				transform2d->setPosition(position);
			}
		}
	}
	
	void Direction2DAspect::setDirection(const Vector2d& direction_arg) {
		if(direction != direction_arg) {
			direction = direction_arg;
			onChangeDirection(direction);
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
	
	void Direction2DAspect::addListener(Direction2DListener* listener) {
		listeners.push_back(listener);
	}
	
	void Direction2DAspect::removeListener(Direction2DListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	void Direction2DAspect::onChangeDirection(Vector2d direction) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onChangeDirection(this, direction);
			}
		}
	}
	
	
	
	void Direction2DListener::onChangeDirection(Direction2DAspect* aspect, Vector2d direction) {
		// open for implementation
	}
}
