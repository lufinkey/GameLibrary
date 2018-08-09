
#pragma once

#include "CollisionRects/CollisionRect.hpp"
#include "ContactEvent.hpp"
#include "CollisionEvent.hpp"
#include <GameLibrary/Application/ApplicationData.hpp>

namespace fgl
{
	struct TransformState
	{
		TransformState(Vector2d position, double rotation)
			: position(position),
			rotation(rotation) {
			//
		}
		
		Vector2d position;
		double rotation;
	};
	
	
	
	class Collidable
	{
		friend class CollisionManager;
	public:
		Collidable();
		
		virtual bool getFlag(const String& flag) const;
		
		virtual TransformState getTransformState() const = 0;
		virtual void shift(const Vector2d& offset) = 0;

		virtual double getMass() const;
		virtual bool isStaticCollisionBody() const = 0;
		virtual ArrayList<const CollisionRect*> getCollisionRects() const = 0;

		TransformState getPreviousTransformState() const;
		Vector2d getDisplacement() const;

		bool checkCollision(Collidable* collidable) const;

		virtual bool respondsToCollision(Collidable* collided, CollisionSide side, CollisionRectPair rectPair) const;
		
		ArrayList<Collidable*> getCollidedOnSide(CollisionSide side);
		ArrayList<const Collidable*> getCollidedOnSide(CollisionSide side) const;
		double getCollidedMassOnSide(CollisionSide side) const;
		bool hasStaticCollisionOnSide(CollisionSide side) const;
		bool hasCollision(const Collidable* collidable) const;
		bool hasCollision(const Collidable* collidable, CollisionSide side) const;

	protected:
		virtual void onBeginCollisionUpdates();

		virtual void onContact(const ContactEvent& contactEvent);
		virtual void onContactUpdate(const ContactEvent& contactEvent);
		virtual void onContactFinish(const ContactEvent& contactEvent);

		virtual void onCollision(const CollisionEvent& collisionEvent);
		virtual void onCollisionUpdate(const CollisionEvent& collisionEvent);
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent);
		
		virtual void updateTransformState();
		
		virtual void onFinishCollisionUpdates();
		
	private:
		void shiftCollisionsOnSide(CollisionSide side, const Vector2d& offset);
		
		TransformState previousTransformState;
		Vector2d displacement;
		std::map<CollisionSide, std::list<Collidable*>> collided;
		std::map<CollisionSide, std::list<Collidable*>> newCollided;
	};
}
