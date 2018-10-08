
#include <GameLibrary/World/Aspects/Drawing/AnimationControllerAspect.hpp>
#include <GameLibrary/World/World.hpp>

namespace fgl
{
	AnimationControllerAspect::AnimationControllerAspect(const std::list<std::pair<String,Animator*>>& animators)
		: animators(animators),
		animationJustFinished(false) {
		//
	}
	
	void AnimationControllerAspect::setAnimators(const std::list<std::pair<String,Animator*>>& animators_arg) {
		animators = animators_arg;
	}
	
	const std::list<std::pair<String,Animator*>>& AnimationControllerAspect::getAnimators() const {
		return animators;
	}
	
	void AnimationControllerAspect::setController(AnimationControllerFunc controller_arg) {
		controller = controller_arg;
	}
	
	const AnimationControllerFunc& AnimationControllerAspect::getController() const {
		return controller;
	}
	
	
	
	
	void AnimationControllerAspect::onAddObjectToWorld(World* world) {
		world->getDrawManager()->addListener(this);
		updateState();
	}
	
	void AnimationControllerAspect::onRemoveObjectFromWorld(World* world) {
		world->getDrawManager()->removeListener(this);
	}
	
	void AnimationControllerAspect::onBeginDrawManagerUpdates(DrawManager* drawManager) {
		updateState();
	}
	
	
	
	
	const String& AnimationControllerAspect::getAnimationName() const {
		return currentState.name;
	}
	
	void AnimationControllerAspect::setState(AnimatorSyncState syncState) {
		auto direction = Animation::Direction::FORWARD;
		if(syncState.state.name == currentState.name) {
			direction = Animation::Direction::NO_CHANGE;
		}
		
		bool didFirstAnimator = false;
		for(auto& pair : animators) {
			auto animator = pair.second;
			
			String animName = pair.first + syncState.state.name;
			
			if(didFirstAnimator) {
				animator->changeAnimation(animName, direction);
			}
			else {
				animator->changeAnimation(animName, direction, [this](AnimatorEvent animatorEvent) {
					if(animatorEvent == AnimatorEvent::FINISHED) {
						animationJustFinished = true;
						updateState();
					}
				});
				didFirstAnimator = true;
			}
			
			if(syncState.sync.frameIndex != -1) {
				animator->getPlayer()->setFrameIndex(syncState.sync.frameIndex);
			}
			if(syncState.sync.lastFrameTime != -1) {
				animator->getPlayer()->setLastFrameTime(syncState.sync.lastFrameTime);
			}
		}
		
		currentState = syncState.state;
	}
	
	
	
	
	AnimatorState AnimationControllerAspect::getState() const {
		return currentState;
	}
	
	AnimatorSyncState AnimationControllerAspect::getSyncState() const {
		auto animator = animators.size() > 0 ? animators.front().second : nullptr;
		auto syncData = AnimatorSync{
			animator ? animator->getFrameIndex() : (size_t)-1,
			animator ? animator->getPlayer()->getLastFrameTime() : (long long)-1 };
		return AnimatorSyncState(currentState, syncData);
	}
	
	void AnimationControllerAspect::updateState() {
		bool animFinished = animationJustFinished;
		animationJustFinished = false;
		if(!controller) {
			return;
		}
		auto prevState = getState();
		auto syncState = controller(prevState, animFinished);
		
		bool stateChanged = false;
		if(syncState.state != prevState || syncState.sync.frameIndex != -1 || syncState.sync.lastFrameTime != -1) {
			stateChanged = true;
			setState(syncState);
		}
	}
}
