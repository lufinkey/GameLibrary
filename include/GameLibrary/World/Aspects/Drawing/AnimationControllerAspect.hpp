
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Animation/Animator.hpp>
#include <GameLibrary/Draw/DrawManager.hpp>

namespace fgl
{
	struct AnimatorState {
		String name = "";
		bool mirrorHorizontal = false;
		bool mirrorVertical = false;
		
		bool equals(const AnimatorState& state) const {
			if(name == state.name && mirrorHorizontal == state.mirrorHorizontal && mirrorVertical == state.mirrorHorizontal) {
				return true;
			}
			return false;
		}
		
		bool operator==(const AnimatorState& state) const {
			return equals(state);
		}
		
		bool operator!=(const AnimatorState& state) const {
			return !equals(state);
		}
	};
	
	struct AnimatorSync {
		size_t frameIndex = -1;
		long long lastFrameTime = -1;
	};
	
	struct AnimatorSyncState {
		AnimatorState state;
		AnimatorSync sync;
		
		AnimatorSyncState(AnimatorState state) : state(state) {}
		AnimatorSyncState(AnimatorState state, AnimatorSync sync) : state(state), sync(sync) {}
		AnimatorSyncState(std::tuple<AnimatorState,AnimatorSync> tuple) : state(std::get<0>(tuple)), sync(std::get<1>(tuple)) {}
	};
	
	typedef std::function<AnimatorSyncState(AnimatorState state, bool finishedAnim)> AnimationControllerFunc;
	
	
	
	
	class AnimationControllerAspect : public WorldObjectAspect, public DrawManagerListener
	{
	public:
		AnimationControllerAspect(const std::list<std::pair<String,Animator*>>& animators = {});
		
		void setAnimators(const std::list<std::pair<String,Animator*>>& animators);
		const std::list<std::pair<String,Animator*>>& getAnimators() const;
		
		template<typename VALUE_TYPE>
		void setProp(const String& key, VALUE_TYPE value) {
			props.set(key, value);
		}
		template<typename VALUE_TYPE>
		const VALUE_TYPE& getProp(const String& key, const VALUE_TYPE& defaultValue) const {
			return fgl::extract<VALUE_TYPE>(key, props, defaultValue);
		}
		
		void setController(AnimationControllerFunc controller);
		const AnimationControllerFunc& getController() const;
		
		const String& getAnimationName() const;
		AnimatorState getState() const;
		AnimatorSyncState getSyncState() const;
		void updateState();
		
	protected:
		virtual void onAddObjectToWorld(World* world) override;
		virtual void onRemoveObjectFromWorld(World* world) override;
		
		virtual void onBeginDrawManagerUpdates(DrawManager* drawManager) override;
		
	private:
		void setState(AnimatorSyncState state);
		
		AnimatorState currentState;
		std::list<std::pair<String,Animator*>> animators;
		
		Dictionary props;
		AnimationControllerFunc controller;
		
		bool animationJustFinished;
	};
}
