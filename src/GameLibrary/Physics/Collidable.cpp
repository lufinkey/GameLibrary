
#include <GameLibrary/Physics/Collidable.hpp>

namespace fgl
{
	Collidable::Collidable()
		: previousTransformState(fgl::Vector2d(0,0), 0),
		displacement(0, 0) {
		//
	}
	
	bool Collidable::getFlag(const String& flag) const {
		if(flag == "Collidable") {
			return true;
		}
		return false;
	}

	double Collidable::getMass() const {
		return 1.0;
	}

	TransformState Collidable::getPreviousTransformState() const {
		return previousTransformState;
	}
	
	Vector2d Collidable::getDisplacement() const {
		return displacement;
	}
	
	ArrayList<Collidable*> Collidable::getCollidedOnSide(CollisionSide side) {
		try {
			return collided.at(side);
		}
		catch(const std::out_of_range& e) {
			return {};
		}
	}
	
	ArrayList<const Collidable*> Collidable::getCollidedOnSide(CollisionSide side) const {
		try {
			return reinterpret_cast<const std::list<const Collidable*>&>(collided.at(side));
		}
		catch(const std::out_of_range& e) {
			return {};
		}
	}
	
	double Collidable::getCollidedMassOnSide(CollisionSide side) const {
		try {
			auto& collidables = collided.at(side);
			double mass = 0.0;
			for(auto collidable : collidables) {
				mass += collidable->getMass();
				mass += collidable->getCollidedMassOnSide(side);
			}
			return mass;
		}
		catch(const std::out_of_range& e) {
			return 0.0;
		}
	}
	
	bool Collidable::hasStaticCollisionOnSide(CollisionSide side) const {
		try {
			auto& collidables = collided.at(side);
			for(auto collidable : collidables) {
				if(collidable->isStaticCollisionBody() || collidable->hasStaticCollisionOnSide(side)) {
					return true;
				}
			}
			return false;
		}
		catch(const std::out_of_range& e) {
			return false;
		}
	}
	
	void Collidable::shiftCollisionsOnSide(CollisionSide side, const Vector2d& offset) {
		try {
			auto& collidables = collided.at(side);
			for(auto collidable : collidables) {
				if(!collidable->isStaticCollisionBody()) {
					collidable->shift(offset);
					collidable->shiftCollisionsOnSide(side, offset);
				}
			}
		}
		catch(const std::out_of_range& e) {
			// do nothing
		}
	}
	
	bool Collidable::hasCollision(const Collidable* collidable) const {
		for(auto& pair : collided) {
			for(auto cmpCollidable : pair.second) {
				if(cmpCollidable == collidable) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool Collidable::hasCollision(const Collidable* collidable, CollisionSide side) const {
		try {
			auto& collidables = collided.at(side);
			for(auto cmpCollidable : collidables) {
				if(cmpCollidable == collidable) {
					return true;
				}
			}
			return false;
		}
		catch(const std::out_of_range& e) {
			return false;
		}
	}

	void Collidable::onContact(const ContactEvent& contactEvent) {
		//
	}

	void Collidable::onContactUpdate(const ContactEvent& contactEvent) {
		//
	}

	void Collidable::onContactFinish(const ContactEvent& contactEvent) {
		//
	}

	bool Collidable::respondsToCollision(Collidable* collided, CollisionSide side, CollisionRectPair rectPair) const {
		return true;
	}

	bool Collidable::checkCollision(Collidable* collidable) const {
		auto collisionRects1 = getCollisionRects();
		auto collisionRects2 = collidable->getCollisionRects();
		for(size_t i=0; i<collisionRects1.size(); i++) {
			for(size_t j=0; j<collisionRects2.size(); j++) {
				if(CollisionRect::checkCollision(this, collisionRects1[i], collidable, collisionRects2[j])) {
					return true;
				}
			}
		}
		return false;
	}

	void Collidable::onCollision(const CollisionEvent& collisionEvent) {
		//
	}

	void Collidable::onCollisionUpdate(const CollisionEvent& collisionEvent) {
		//
	}

	void Collidable::onCollisionFinish(const CollisionEvent& collisionEvent) {
		//
	}

	void Collidable::onBeginCollisionUpdates() {
		//
	}
	
	void Collidable::updateTransformState() {
		auto transformState = getTransformState();
		auto prevTransformState = getPreviousTransformState();
		displacement = (transformState.position - prevTransformState.position);
		previousTransformState = transformState;
	}

	void Collidable::onFinishCollisionUpdates() {
		//
	}
}
