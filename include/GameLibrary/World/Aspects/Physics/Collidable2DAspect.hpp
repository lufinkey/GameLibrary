
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Physics/Collidable.hpp>

namespace fgl
{
	class Collision2DDelegate;
	typedef TransformState TransformState2D;
	
	class Collidable2DAspect : public WorldObjectAspect, public Collidable
	{
	public:
		Collidable2DAspect(double mass=1.0);
		virtual ~Collidable2DAspect();
		
		virtual String getDebugDescription() const override;
		virtual bool getFlag(const String& flag) const override;
		
		virtual TransformState2D getTransformState() const override;
		virtual void shift(const Vector2d& offset) override;
		virtual bool respondsToCollision(const Collidable* collidable, CollisionSide side, CollisionRectPair rectPair) const override;
		
		virtual bool isStaticCollisionBody() const override final;
		void setStaticCollisionBody(bool staticCollision);
		
		virtual double getMass() const override final;
		double getIndividualMass() const;
		void setIndividualMass(double mass);
		
		void setAsSensor(bool sensor);
		bool isSensor() const override final;
		
		void setNeedsNewCollisionRects();
		virtual ArrayList<const CollisionRect*> getCollisionRects() const override;
		
		void addDelegate(Collision2DDelegate* delegate);
		void removeDelegate(Collision2DDelegate* delegate);
		
	protected:
		virtual void onAddObjectToWorld(World* world) override;
		virtual void onRemoveObjectFromWorld(World* world) override;
		
		virtual bool shouldEnableCollisions() const;
		virtual ArrayList<const CollisionRect*> createCollisionRects() const = 0;
		
		virtual void onBeginCollisionUpdates(const ApplicationData& appData) override;
		
	private:
		ArrayList<const CollisionRect*> collisionRects;
		std::list<Collision2DDelegate*> delegates;
		double mass;
		bool staticCollision;
		bool sensor;
		bool needsNewCollisionRects;
	};
	
	
	
	class Collision2DDelegate
	{
	public:
		virtual ~Collision2DDelegate() = default;
		
		virtual bool shouldEnableCollisions(const Collidable2DAspect* aspect) const { return true; }
		virtual bool respondsToCollision(const Collidable2DAspect* aspect, const Collidable* collidable, CollisionSide side, CollisionRectPair rectPair) const { return true; }
	};
}
