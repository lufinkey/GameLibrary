
#pragma once

#include <GameLibrary/World/World.hpp>
#include <GameLibrary/Utilities/Geometry/Vector3.hpp>
#include <list>

namespace fgl
{
	class Transform3DAspect;
	
	class Transform3DOffsetProvider
	{
	public:
		virtual ~Transform3DOffsetProvider() = default;
		
		virtual Vector3d getTransform3DOffset() const;
		virtual Vector3d getChildTransform3DOffset(const Transform3DAspect* childAspect) const;
	};
	
	
	
	class Transform3DAspect : public WorldObjectAspect
	{
	public:
		explicit Transform3DAspect(const Vector3d& position=Vector3d(0,0,0), bool listenerEvents = false, bool usesOffsets = false);
		
		void setPosition(const Vector3d& position);
		Vector3d getPosition() const;
		
		void setLocalPosition(const Vector3d& position);
		Vector3d getLocalPosition() const;
		
		void attachChild(Transform3DAspect* childTransform, bool keepGlobalPosition = true);
		void attachChild(Transform3DAspect* childTransform, const Vector3d& localPosition);
		void detachChild(Transform3DAspect* childTransform, bool keepGlobalPosition = true);
		
		const std::list<Transform3DAspect*>& getChildren();
		const std::list<const Transform3DAspect*>& getChildren() const;
		Transform3DAspect* getParent();
		const Transform3DAspect* getParent() const;
		
		bool listenerEventsEnabled() const;
		void setListenerEventsEnabled(bool listenerEvents);
		
		void setUsesOffsetProviders(bool usesOffsets);
		bool usesOffsetProviders() const;
		
	protected:
		virtual void onAttachToParentTransform(Transform3DAspect* parent);
		virtual void onDetachFromParentTransform(Transform3DAspect* parent);
		
		virtual void onAttachChildTransform(Transform3DAspect* child);
		virtual void onDetachChildTransform(Transform3DAspect* child);
		
		virtual void onChangePosition(Vector3d position);
		
	private:
		inline Vector3d getTransformOffsets() const {
			auto offset = Vector3d(0,0,0);
			if(usesOffsets) {
				for(auto offsetProvider : getAspects<Transform3DOffsetProvider>()) {
					offset += offsetProvider->getTransform3DOffset();
				}
			}
			return offset;
		}
		
		inline Vector3d getChildTransformOffsets(const Transform3DAspect* child) const {
			auto offset = Vector3d(0,0,0);
			if(usesOffsets) {
				for(auto offsetProvider : getAspects<Transform3DOffsetProvider>()) {
					offset += offsetProvider->getChildTransform3DOffset(child);
				}
			}
			return offset;
		}
		
		Vector3d localPosition;
		Transform3DAspect* parent;
		std::list<Transform3DAspect*> children;
		bool listenerEvents;
		bool usesOffsets;
	};
	
	
	
	class Transform3DListener
	{
		friend class Transform3DAspect;
	public:
		virtual ~Transform3DListener() = default;
		
	protected:
		virtual void onAttachToParentTransform(Transform3DAspect* parent);
		virtual void onDetachFromParentTransform(Transform3DAspect* parent);
		
		virtual void onAttachChildTransform(Transform3DAspect* child);
		virtual void onDetachChildTransform(Transform3DAspect* child);
		
		virtual void onChangePosition(Vector3d position);
	};
}
