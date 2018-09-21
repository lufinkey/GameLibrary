
#include <GameLibrary/World/Aspects/Physics/Collidable2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Velocity2DAspect.hpp>
#include <GameLibrary/World/World.hpp>

namespace fgl
{
	Collidable2DAspect::Collidable2DAspect(double mass)
		: mass(mass),
		staticCollision(false),
		sensor(false),
		needsNewCollisionRects(true) {
		//
	}
	
	Collidable2DAspect::~Collidable2DAspect() {
		for(auto rect : collisionRects) {
			delete rect;
		}
	}
	
	String Collidable2DAspect::getDebugDescription() const {
		auto object = getObject();
		if(object == nullptr) {
			return (String)"Collidable2DAspect @ "+getTransformState().position.toString();
		}
		return (String)"Collidable2DAspect of "+object->getDebugDescription();
	}
	
	bool Collidable2DAspect::getFlag(const String& flag) const {
		if(flag == "Collidable2DAspect") {
			return true;
		}
		return WorldObjectAspect::getFlag(flag) || Collidable::getFlag(flag);
	}
	
	TransformState2D Collidable2DAspect::getTransformState() const {
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d == nullptr) {
			throw IllegalStateException("Collidable3DAspect requires a Transform3DAspect");
		}
		return TransformState2D(transform2d->getPosition(), 0);
	}
	
	void Collidable2DAspect::shift(const Vector2d& offset) {
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d == nullptr) {
			throw IllegalStateException("Collidable2DAspect requires a Transform3DAspect");
		}
		while(transform2d->getParent() != nullptr) {
			transform2d = transform2d->getParent();
		}
		transform2d->setPosition(transform2d->getPosition() + offset);
	}
	
	bool Collidable2DAspect::respondsToCollision(const Collidable* collidable, CollisionSide side, CollisionRectPair rectPair) const {
		if(!Collidable::respondsToCollision(collidable, side, rectPair)) {
			return false;
		}
		for(auto delegate : delegates) {
			if(!delegate->respondsToCollision(this, collidable, side, rectPair)) {
				return false;
			}
		}
		return true;
	}
	
	bool Collidable2DAspect::isStaticCollisionBody() const {
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d == nullptr) {
			throw IllegalStateException("Collidable3DAspect requires a Transform3DAspect");
		}
		auto parentTransform2d = transform2d->getParent();
		if(parentTransform2d != nullptr) {
			auto parentCollidable = parentTransform2d->getAspect<Collidable>();
			if(parentCollidable != nullptr) {
				return parentCollidable->isStaticCollisionBody();
			}
		}
		return staticCollision;
	}
	
	void Collidable2DAspect::setStaticCollisionBody(bool staticCollision_arg) {
		staticCollision = staticCollision_arg;
	}
	
	double Collidable2DAspect::getMass() const {
		double totalMass = mass;
		auto transform2d = getAspect<Transform2DAspect>();
		if(transform2d != nullptr) {
			for(auto childTransform : transform2d->getChildren()) {
				for(auto collidable : childTransform->getAspects<Collidable>()) {
					totalMass += collidable->getMass();
				}
			}
		}
		return totalMass;
	}
	
	double Collidable2DAspect::getIndividualMass() const {
		return mass;
	}
	
	void Collidable2DAspect::setIndividualMass(double mass_arg) {
		mass = mass_arg;
	}
	
	bool Collidable2DAspect::isSensor() const {
		return sensor;
	}
	
	void Collidable2DAspect::setAsSensor(bool sensor_arg) {
		sensor = sensor_arg;
	}
	
	void Collidable2DAspect::setNeedsNewCollisionRects() {
		needsNewCollisionRects = true;
	}
	
	
	
	
	
	void Collidable2DAspect::addDelegate(Collision2DDelegate* delegate) {
		delegates.push_back(delegate);
	}
	
	void Collidable2DAspect::removeDelegate(Collision2DDelegate* delegate) {
		auto delegateIt = std::find(delegates.begin(), delegates.end(), delegate);
		if(delegateIt != delegates.end()) {
			delegates.erase(delegateIt);
		}
	}
	
	
	
	
	
	void Collidable2DAspect::onAddObjectToWorld(World* world) {
		world->addCollidable(this);
	}
	
	void Collidable2DAspect::onRemoveObjectFromWorld(World* world) {
		world->removeCollidable(this);
	}
	
	
	
	
	
	ArrayList<const CollisionRect*> Collidable2DAspect::getCollisionRects() const {
		return collisionRects;
	}
	
	
	bool Collidable2DAspect::shouldEnableCollisions() const {
		if(delegates.size() > 0) {
			for(auto delegate : delegates) {
				if(!delegate->shouldEnableCollisions(this)) {
					return false;
				}
			}
		}
		return true;
	}
	
	
	void Collidable2DAspect::onBeginCollisionUpdates(const ApplicationData& appData) {
		bool enableCollisions = shouldEnableCollisions();
		if((!enableCollisions && collisionRects.size() > 0) || (enableCollisions && collisionRects.size() == 0)) {
			setNeedsNewCollisionRects();
		}
		
		bool neededNewCollisionRects = needsNewCollisionRects;
		if(needsNewCollisionRects) {
			auto prevCollisionRects = collisionRects;
			if(enableCollisions) {
				collisionRects = createCollisionRects();
			}
			else {
				collisionRects = {};
			}
			for(auto collisionRect : prevCollisionRects) {
				delete collisionRect;
			}
			needsNewCollisionRects = false;
		}
		auto velocity2d = getAspect<Velocity2DAspect>();
		if(neededNewCollisionRects || getTransformState() != getPreviousTransformState() || (velocity2d != nullptr && velocity2d->getVelocity() != Vector2d(0,0))) {
			wake();
		}
		else {
			sleep();
		}
	}
}
