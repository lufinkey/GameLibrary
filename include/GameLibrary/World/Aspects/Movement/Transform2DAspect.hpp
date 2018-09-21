
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <list>

namespace fgl
{
	class Transform2DAspect;
	class Transform2DListener;
	class Transform2DOffsetProvider;
	
	
	
	class Transform2DAspect : public WorldObjectAspect
	{
	public:
		explicit Transform2DAspect(const Vector2d& position=Vector2d(0,0));
		
		void setPosition(const Vector2d& position);
		Vector2d getPosition() const;
		
		void setLocalPosition(const Vector2d& position);
		Vector2d getLocalPosition() const;
		
		void attachChild(Transform2DAspect* childTransform, bool keepGlobalPosition = true);
		void attachChild(Transform2DAspect* childTransform, const Vector2d& localPosition);
		void detachChild(Transform2DAspect* childTransform, bool keepGlobalPosition = true);
		
		const std::list<Transform2DAspect*>& getChildren();
		const std::list<const Transform2DAspect*>& getChildren() const;
		Transform2DAspect* getParent();
		const Transform2DAspect* getParent() const;
		
		void addListener(Transform2DListener* listener);
		void removeListener(Transform2DListener* listener);
		
		void addOffsetProvider(Transform2DOffsetProvider* offsetProvider);
		void removeOffsetProvider(Transform2DOffsetProvider* offsetProvider);
		
	protected:
		virtual void onAttachToParentTransform(Transform2DAspect* parent);
		virtual void onDetachFromParentTransform(Transform2DAspect* parent);
		
		virtual void onAttachChildTransform(Transform2DAspect* child);
		virtual void onDetachChildTransform(Transform2DAspect* child);
		
		virtual void onChangePosition(Vector2d position);
		
	private:
		Vector2d getTransformOffsets() const;
		Vector2d getChildTransformOffsets(const Transform2DAspect* child) const;
		
		Vector2d localPosition;
		Transform2DAspect* parent;
		std::list<Transform2DAspect*> children;
		
		std::list<Transform2DListener*> listeners;
		std::list<Transform2DOffsetProvider*> offsetProviders;
	};
	
	
	
	
	class Transform2DListener
	{
		friend class Transform2DAspect;
	public:
		virtual ~Transform2DListener() = default;
		
	protected:
		virtual void onAttachToParentTransform(Transform2DAspect* parent);
		virtual void onDetachFromParentTransform(Transform2DAspect* parent);
		
		virtual void onAttachChildTransform(Transform2DAspect* child);
		virtual void onDetachChildTransform(Transform2DAspect* child);
		
		virtual void onChangePosition(Vector2d position);
	};
	
	
	
	
	class Transform2DOffsetProvider
	{
	public:
		virtual ~Transform2DOffsetProvider() = default;
		
		virtual Vector2d getTransform2DOffset(const Transform2DAspect* transform2d) const;
		virtual Vector2d getChildTransform2DOffset(const Transform2DAspect* transform2d, const Transform2DAspect* childTransform2d) const;
	};
}
