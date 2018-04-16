
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
			rotation(rotation)
		{
			//
		}
		
		Vector2d position;
		double rotation;
	};
	
	class Collidable
	{
		friend class CollisionManager;
	public:
		Collidable(const TransformState& initialState);
		
		virtual bool getFlag(const String& flag) const;
		
		virtual TransformState getTransformState() const = 0;
		virtual void shift(const Vector2d& offset) = 0;

		virtual double getMass() const;
		virtual bool isStaticCollisionBody() const = 0;
		virtual ArrayList<CollisionRect*> getCollisionRects() const = 0;

		TransformState getPreviousTransformState() const;

		void setVelocity(const Vector2d& velocity);
		Vector2d getVelocity() const;

		Vector2d getDisplacement() const;

		void applyForce(const Vector2d& force);

		bool checkCollision(Collidable* collidable) const;

		virtual bool respondsToCollision(Collidable* collided, CollisionSide side) const;

	protected:
		virtual void onBeginCollisionUpdates();

		virtual void onContact(const ContactEvent& contactEvent);
		virtual void onContactUpdate(const ContactEvent& contactEvent);
		virtual void onContactFinish(const ContactEvent& contactEvent);

		virtual void onCollision(const CollisionEvent& collisionEvent);
		virtual void onCollisionUpdate(const CollisionEvent& collisionEvent);
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent);

		virtual void onFinishCollisionUpdates();

	private:
		void updateVelocity(double frameSpeedMultiplier);
		
		TransformState previousTransformState;
		Vector2d velocity;
		Vector2d displacement;
	};
}
