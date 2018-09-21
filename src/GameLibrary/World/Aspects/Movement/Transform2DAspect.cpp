
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>

namespace fgl
{
	Transform2DAspect::Transform2DAspect(const Vector2d& position)
		: localPosition(position),
		parent(nullptr) {
		//
	}
	
	void Transform2DAspect::setPosition(const Vector2d& position) {
		auto offset = getTransformOffsets();
		if(parent != nullptr) {
			setLocalPosition(position - (parent->getPosition() + parent->getChildTransformOffsets(this) + offset));
		}
		else {
			setLocalPosition(position - offset);
		}
	}
	
	Vector2d Transform2DAspect::getPosition() const {
		auto offset = getTransformOffsets();
		if(parent != nullptr) {
			return parent->getPosition() + parent->getChildTransformOffsets(this) + localPosition + offset;
		}
		else {
			return localPosition + offset;
		}
	}
	
	void Transform2DAspect::setLocalPosition(const Vector2d& position) {
		localPosition = position;
		onChangePosition(getPosition());
	}
	
	Vector2d Transform2DAspect::getLocalPosition() const {
		return localPosition;
	}
	
	void Transform2DAspect::attachChild(Transform2DAspect* childTransform, bool keepGlobalPosition) {
		auto offset = childTransform->getLocalPosition();
		if(keepGlobalPosition) {
			offset -= getPosition();
		}
		attachChild(childTransform, offset);
	}
	
	void Transform2DAspect::attachChild(Transform2DAspect* childTransform, const Vector2d& offset) {
		if(childTransform->parent != nullptr) {
			throw IllegalArgumentException("childTransform", "already attached to parent transform");
		}
		childTransform->parent = this;
		children.push_back(childTransform);
		childTransform->setLocalPosition(offset);
		
		childTransform->onAttachToParentTransform(this);
		onAttachChildTransform(childTransform);
	}
	
	void Transform2DAspect::detachChild(Transform2DAspect* childTransform, bool keepGlobalPosition) {
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
	
	const std::list<Transform2DAspect*>& Transform2DAspect::getChildren() {
		return children;
	}
	
	const std::list<const Transform2DAspect*>& Transform2DAspect::getChildren() const {
		return reinterpret_cast<const std::list<const Transform2DAspect*>&>(children);
	}
	
	Transform2DAspect* Transform2DAspect::getParent() {
		return parent;
	}
	
	const Transform2DAspect* Transform2DAspect::getParent() const {
		return parent;
	}
	
	void Transform2DAspect::addListener(Transform2DListener* listener) {
		listeners.push_back(listener);
	}
	
	void Transform2DAspect::removeListener(Transform2DListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	void Transform2DAspect::addOffsetProvider(Transform2DOffsetProvider* offsetProvider) {
		offsetProviders.push_back(offsetProvider);
	}
	
	void Transform2DAspect::removeOffsetProvider(Transform2DOffsetProvider* offsetProvider) {
		auto providerIt = std::find(offsetProviders.begin(), offsetProviders.end(), offsetProvider);
		if(providerIt != offsetProviders.end()) {
			offsetProviders.erase(providerIt);
		}
	}
	
	Vector2d Transform2DAspect::getTransformOffsets() const {
		auto offset = Vector2d(0,0);
		for(auto offsetProvider : offsetProviders) {
			offset += offsetProvider->getTransform2DOffset(this);
		}
		return offset;
	}
	
	Vector2d Transform2DAspect::getChildTransformOffsets(const Transform2DAspect* child) const {
		auto offset = Vector2d(0,0);
		for(auto offsetProvider : offsetProviders) {
			offset += offsetProvider->getChildTransform2DOffset(this, child);
		}
		return offset;
	}
	
	void Transform2DAspect::onAttachToParentTransform(Transform2DAspect* parent) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onAttachToParentTransform(parent);
			}
		}
	}
	
	void Transform2DAspect::onDetachFromParentTransform(Transform2DAspect* parent) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onDetachFromParentTransform(parent);
			}
		}
	}
	
	void Transform2DAspect::onAttachChildTransform(Transform2DAspect* child) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onAttachChildTransform(parent);
			}
		}
	}
	
	void Transform2DAspect::onDetachChildTransform(Transform2DAspect* child) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onDetachChildTransform(parent);
			}
		}
	}
	
	void Transform2DAspect::onChangePosition(Vector2d position) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onChangePosition(position);
			}
		}
	}
	
	
	
	
	void Transform2DListener::onAttachToParentTransform(Transform2DAspect* parent) {
		// open for implementation
	}
	
	void Transform2DListener::onDetachFromParentTransform(Transform2DAspect* parent) {
		// open for implementation
	}
	
	void Transform2DListener::onAttachChildTransform(Transform2DAspect* child) {
		// open for implementation
	}
	
	void Transform2DListener::onDetachChildTransform(Transform2DAspect* child) {
		// open for implementation
	}
	
	void Transform2DListener::onChangePosition(Vector2d position) {
		// open for implementation
	}
	
	
	
	
	Vector2d Transform2DOffsetProvider::getTransform2DOffset(const Transform2DAspect* transform2d) const {
		return {0, 0};
	}
	
	Vector2d Transform2DOffsetProvider::getChildTransform2DOffset(const Transform2DAspect* transform2d, const Transform2DAspect* childTransform2d) const {
		return {0, 0};
	}
}
