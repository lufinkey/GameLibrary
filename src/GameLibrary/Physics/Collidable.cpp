
#include <GameLibrary/Physics/Collidable.hpp>

namespace fgl
{
	Collidable::Collidable(const TransformState& initialState)
		: previousTransformState(initialState),
		displacement(0, 0)
	{
		//
	}
	
	bool Collidable::getFlag(const String& flag) const
	{
		if(flag == "Collidable")
		{
			return true;
		}
		return false;
	}

	double Collidable::getMass() const
	{
		return 1.0;
	}

	TransformState Collidable::getPreviousTransformState() const
	{
		return previousTransformState;
	}

	Vector2d Collidable::getDisplacement() const
	{
		return displacement;
	}

	void Collidable::onContact(const ContactEvent& contactEvent)
	{
		//
	}

	void Collidable::onContactUpdate(const ContactEvent& contactEvent)
	{
		//
	}

	void Collidable::onContactFinish(const ContactEvent& contactEvent)
	{
		//
	}

	bool Collidable::respondsToCollision(Collidable* collided, CollisionSide side, CollisionRectPair rectPair) const
	{
		return true;
	}

	bool Collidable::checkCollision(Collidable* collidable) const
	{
		auto collisionRects1 = getCollisionRects();
		auto collisionRects2 = collidable->getCollisionRects();
		for(size_t i=0; i<collisionRects1.size(); i++)
		{
			for(size_t j=0; j<collisionRects2.size(); j++)
			{
				if(CollisionRect::checkCollision(collisionRects1[i], collisionRects2[j]))
				{
					return true;
				}
			}
		}
		return false;
	}

	void Collidable::onCollision(const CollisionEvent& collisionEvent)
	{
		//
	}

	void Collidable::onCollisionUpdate(const CollisionEvent& collisionEvent)
	{
		//
	}

	void Collidable::onCollisionFinish(const CollisionEvent& collisionEvent)
	{
		//
	}

	void Collidable::onBeginCollisionUpdates()
	{
		//
	}
	
	void Collidable::updateTransformState()
	{
		auto transformState = getTransformState();
		auto prevTransformState = getPreviousTransformState();
		displacement = (transformState.position - prevTransformState.position);
		previousTransformState = transformState;
	}

	void Collidable::onFinishCollisionUpdates()
	{
		//
	}
}
