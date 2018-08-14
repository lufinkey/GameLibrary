
#include <GameLibrary/Physics/Collidable.hpp>

namespace fgl
{
	Collidable::Collidable()
		: previousTransformState(fgl::Vector2d(0,0), 0),
		displacement(0, 0) {
		//
	}
	
	String Collidable::getDebugDescription() const {
		return (String)"Collidable @ "+getTransformState().position.toString();
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
	
	RectangleD Collidable::getBoundingRect() const {
		auto transformState = getTransformState();
		auto collisionRects = getCollisionRects();
		if(collisionRects.size() == 0) {
			return RectangleD(transformState.position.x, transformState.position.y, 0, 0);
		}
		auto rect = collisionRects[0]->getRect();
		for(size_t i=1; i<collisionRects.size(); i++) {
			rect.combine(collisionRects[i]->getRect());
		}
		return rect.translated(transformState.position);
	}
	
	ArrayList<Collidable*> Collidable::getCollidedOnSide(CollisionSide side) {
		fgl::ArrayList<Collidable*> collidables;
		try {
			auto& collidedList = collided.at(side);
			collidables.getVector().insert(collidables.end(), collidedList.begin(), collidedList.end());
		}
		catch(const std::out_of_range& e) {
			//
		}
		try {
			auto& collidedList = newCollided.at(side);
			collidables.reserve(collidedList.size());
			for(auto collidable : collidedList) {
				if(!collidables.contains(collidable)) {
					collidables.add(collidable);
				}
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		return collidables;
	}
	
	ArrayList<const Collidable*> Collidable::getCollidedOnSide(CollisionSide side) const {
		fgl::ArrayList<const Collidable*> collidables;
		try {
			auto& collidedList = reinterpret_cast<const std::list<const Collidable*>&>(collided.at(side));
			collidables.getVector().insert(collidables.end(), collidedList.begin(), collidedList.end());
		}
		catch(const std::out_of_range& e) {
			//
		}
		try {
			auto& collidedList = newCollided.at(side);
			collidables.reserve(collidedList.size());
			for(auto collidable : collidedList) {
				if(!collidables.contains(collidable)) {
					collidables.add(collidable);
				}
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		return collidables;
	}
	
	double Collidable::getCollidedMassOnSide(CollisionSide side) const {
		double mass = 0.0;
		const std::list<Collidable*>* collidablesPtr = nullptr;
		try {
			auto& collidables = collided.at(side);
			collidablesPtr = &collidables;
			for(auto collidable : collidables) {
				mass += collidable->getMass();
				mass += collidable->getCollidedMassOnSide(side);
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		try {
			auto& newCollidables = newCollided.at(side);
			for(auto collidable : newCollidables) {
				if(collidablesPtr != nullptr) {
					bool foundMatch = false;
					for(auto cmpCollidable : *collidablesPtr) {
						if(collidable == cmpCollidable) {
							foundMatch = true;
							break;
						}
					}
					if(foundMatch) {
						continue;
					}
				}
				mass += collidable->getMass();
				mass += collidable->getCollidedMassOnSide(side);
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		return mass;
	}
	
	bool Collidable::hasStaticCollisionOnSide(CollisionSide side) const {
		try {
			auto& collidables = collided.at(side);
			for(auto collidable : collidables) {
				if(collidable->isStaticCollisionBody() || collidable->hasStaticCollisionOnSide(side)) {
					return true;
				}
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		try {
			auto& collidables = newCollided.at(side);
			for(auto collidable : collidables) {
				if(collidable->isStaticCollisionBody() || collidable->hasStaticCollisionOnSide(side)) {
					return true;
				}
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		return false;
	}
	
	void Collidable::shiftCollisionsOnSide(CollisionSide side, const Vector2d& offset) {
		std::list<Collidable*>* collidablesPtr = nullptr;
		try {
			auto& collidables = collided.at(side);
			collidablesPtr = &collidables;
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
		try {
			auto& newCollidables = newCollided.at(side);
			for(auto collidable : newCollidables) {
				if(collidablesPtr != nullptr) {
					bool foundMatch = false;
					if(collidablesPtr != nullptr) {
						for(auto cmpCollidable : *collidablesPtr) {
							if(collidable == cmpCollidable) {
								foundMatch = true;
								break;
							}
						}
					}
					if(foundMatch) {
						continue;
					}
				}
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
		for(auto& pair : newCollided) {
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
		}
		catch(const std::out_of_range& e) {
			//
		}
		try {
			auto& collidables = newCollided.at(side);
			for(auto cmpCollidable : collidables) {
				if(cmpCollidable == collidable) {
					return true;
				}
			}
		}
		catch(const std::out_of_range& e) {
			//
		}
		return false;
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
