
#include <GameLibrary/World/Aspects/Movement/Transform3DAspect.hpp>

namespace fgl
{
	Vector3d Transform3DOffsetProvider::getTransform3DOffset() const {
		return {0, 0, 0};
	}
	
	Vector3d Transform3DOffsetProvider::getChildTransform3DOffset(const Transform3DAspect* childAspect) const {
		return {0, 0, 0};
	}
	
	Transform3DAspect::Transform3DAspect(const Vector3d& position, bool listenerEvents, bool usesOffsets)
		: localPosition(position),
		parent(nullptr),
		listenerEvents(listenerEvents),
		usesOffsets(usesOffsets) {
		//
	}
	
	void Transform3DAspect::setPosition(const Vector3d& position) {
		auto offset = getTransformOffsets();
		if(parent != nullptr) {
			setLocalPosition(position - (parent->getPosition() + parent->getChildTransformOffsets(this) + offset));
		}
		else {
			setLocalPosition(position - offset);
		}
	}
	
	Vector3d Transform3DAspect::getPosition() const {
		auto offset = getTransformOffsets();
		if(parent != nullptr) {
			return parent->getPosition() + parent->getChildTransformOffsets(this) + localPosition + offset;
		}
		else {
			return localPosition + offset;
		}
	}
	
	void Transform3DAspect::setLocalPosition(const Vector3d& position) {
		localPosition = position;
		onChangePosition(getPosition());
	}
	
	Vector3d Transform3DAspect::getLocalPosition() const {
		return localPosition;
	}
	
	void Transform3DAspect::attachChild(Transform3DAspect* childTransform, bool keepGlobalPosition) {
		auto offset = childTransform->getLocalPosition();
		if(keepGlobalPosition) {
			offset -= getPosition();
		}
		attachChild(childTransform, offset);
	}
	
	void Transform3DAspect::attachChild(Transform3DAspect* childTransform, const Vector3d& offset) {
		if(childTransform->parent != nullptr) {
			throw IllegalArgumentException("childTransform", "already attached to parent transform");
		}
		childTransform->parent = this;
		children.push_back(childTransform);
		childTransform->setLocalPosition(offset);
		
		childTransform->onAttachToParentTransform(this);
		onAttachChildTransform(childTransform);
	}
	
	void Transform3DAspect::detachChild(Transform3DAspect* childTransform, bool keepGlobalPosition) {
		if(childTransform->parent == nullptr || childTransform->parent != this) {
			return;
		}
		auto position = childTransform->getLocalPosition();
		auto parentedOffsets = childTransform->getTransformOffsets();
		auto childOffsets = getChildTransformOffsets(childTransform);
		if(keepGlobalPosition) {
			position += getPosition();
		}
		childTransform->parent = nullptr;
		auto childIt = std::find(children.begin(), children.end(), childTransform);
		if(childIt != children.end()) {
			children.erase(childIt);
		}
		auto nonParentedOffsets = childTransform->getTransformOffsets();
		childTransform->setLocalPosition(position + (parentedOffsets + childOffsets) - nonParentedOffsets);
		
		onDetachChildTransform(childTransform);
		childTransform->onDetachFromParentTransform(this);
	}
	
	const std::list<Transform3DAspect*>& Transform3DAspect::getChildren() {
		return children;
	}
	
	const std::list<const Transform3DAspect*>& Transform3DAspect::getChildren() const {
		return reinterpret_cast<const std::list<const Transform3DAspect*>&>(children);
	}
	
	Transform3DAspect* Transform3DAspect::getParent() {
		return parent;
	}
	
	const Transform3DAspect* Transform3DAspect::getParent() const {
		return parent;
	}
	
	bool Transform3DAspect::listenerEventsEnabled() const {
		return listenerEvents;
	}
	
	void Transform3DAspect::setListenerEventsEnabled(bool listenerEvents_arg) {
		listenerEvents = listenerEvents_arg;
	}
	
	bool Transform3DAspect::usesOffsetProviders() const {
		return usesOffsets;
	}
	
	void Transform3DAspect::setUsesOffsetProviders(bool usesOffsets_arg) {
		usesOffsets = usesOffsets_arg;
	}
	
	void Transform3DAspect::onAttachToParentTransform(Transform3DAspect* parent) {
		if(listenerEvents) {
			for(auto listener : getAspects<Transform3DListener>()) {
				listener->onAttachToParentTransform(parent);
			}
		}
	}
	
	void Transform3DAspect::onDetachFromParentTransform(Transform3DAspect* parent) {
		if(listenerEvents) {
			for(auto listener : getAspects<Transform3DListener>()) {
				listener->onDetachFromParentTransform(parent);
			}
		}
	}
	
	void Transform3DAspect::onAttachChildTransform(Transform3DAspect* child) {
		if(listenerEvents) {
			for(auto listener : getAspects<Transform3DListener>()) {
				listener->onAttachChildTransform(parent);
			}
		}
	}
	
	void Transform3DAspect::onDetachChildTransform(Transform3DAspect* child) {
		if(listenerEvents) {
			for(auto listener : getAspects<Transform3DListener>()) {
				listener->onDetachChildTransform(parent);
			}
		}
	}
	
	void Transform3DAspect::onChangePosition(Vector3d position) {
		if(listenerEvents) {
			for(auto listener : getAspects<Transform3DListener>()) {
				listener->onChangePosition(position);
			}
		}
	}
	
	
	
	
	void Transform3DListener::onAttachToParentTransform(Transform3DAspect* parent) {
		// open for implementation
	}
	
	void Transform3DListener::onDetachFromParentTransform(Transform3DAspect* parent) {
		// open for implementation
	}
	
	void Transform3DListener::onAttachChildTransform(Transform3DAspect* child) {
		// open for implementation
	}
	
	void Transform3DListener::onDetachChildTransform(Transform3DAspect* child) {
		// open for implementation
	}
	
	void Transform3DListener::onChangePosition(Vector3d position) {
		// open for implementation
	}
}
