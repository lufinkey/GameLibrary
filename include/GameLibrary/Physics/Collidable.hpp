
#pragma once

#include "CollisionRects/CollisionRect.hpp"
#include "ContactEventListener.hpp"
#include "CollisionEventListener.hpp"
#include "TransformState.hpp"
#include <GameLibrary/Application/ApplicationData.hpp>

namespace fgl
{
	class Collidable
	{
		friend class CollisionManager;
	public:
		Collidable();
		
		virtual String getDebugDescription() const;
		virtual bool getFlag(const String& flag) const;
		
		virtual TransformState getTransformState() const = 0;
		virtual void shift(const Vector2d& offset) = 0;

		virtual double getMass() const;
		virtual bool isStaticCollisionBody() const = 0;
		virtual ArrayList<const CollisionRect*> getCollisionRects() const = 0;
		virtual bool isSensor() const;

		TransformState getPreviousTransformState() const;
		Vector2d getDisplacement() const;
		
		RectangleD getBoundingRect() const;

		bool checkCollision(Collidable* collidable) const;

		virtual bool respondsToCollision(const Collidable* collided, CollisionSide side, CollisionRectPair rectPair) const;
		
		ArrayList<Collidable*> getCollidedOnSide(CollisionSide side);
		ArrayList<const Collidable*> getCollidedOnSide(CollisionSide side) const;
		size_t getCollidedCountOnSide(CollisionSide side) const;
		double getCollidedMassOnSide(CollisionSide side) const;
		bool hasStaticCollisionOnSide(CollisionSide side) const;
		bool hasCollision(const Collidable* collidable) const;
		bool hasCollision(const Collidable* collidable, CollisionSide side) const;
		bool hasCollision(CollisionSide side) const;
		
		bool isAwake() const;
		void wake();
		void sleep();
		
		void addCollisionEventListener(CollisionEventListener* listener);
		void removeCollisionEventListener(CollisionEventListener* listener);
		
		void addContactEventListener(ContactEventListener* listener);
		void removeContactEventListener(ContactEventListener* listener);

	protected:
		virtual void onBeginCollisionUpdates(const ApplicationData& appData);
		virtual void updateTransformState();
		virtual void onFinishCollisionUpdates(const ApplicationData& appData);
		
	private:
		void shiftCollisionsOnSide(CollisionSide side, const Vector2d& offset);
		
		TransformState previousTransformState;
		Vector2d displacement;
		std::map<CollisionSide, std::list<Collidable*>> collided;
		std::map<CollisionSide, std::list<Collidable*>> newCollided;
		RectangleD awarenessRect;
		
		std::list<CollisionEventListener*> collisionEventListeners;
		std::list<ContactEventListener*> contactEventListeners;
		
		bool awake;
	};
}
