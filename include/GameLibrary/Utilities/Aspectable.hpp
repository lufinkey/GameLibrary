
#pragma once

#include "ArrayList.hpp"

namespace fgl
{
	#define ENABLE_IF_EXTENDS(BASE_CLASS, CLASS) typename std::enable_if<std::is_base_of<BASE_CLASS, CLASS>::value, std::nullptr_t>::type = nullptr
	
	template<typename ASPECT>
	class Aspectable
	{
	public:
		using AspectType = ASPECT;
		
		Aspectable() {
			//
		}
		
		virtual ~Aspectable() {
			for(auto aspect : aspects) {
				delete aspect;
			}
		}
		
		Aspectable* addAspect(ASPECT* aspect) {
			aspects.push_back(aspect);
			onAddAspect(aspect);
			return this;
		}
		
		template<typename CLASS, size_t INDEX = 0>
		Aspectable* addAspectBefore(ASPECT* aspect, bool appendIfIndexMissing = false) {
			auto begin = aspects.begin();
			auto end = aspects.end();
			size_t count = 0;
			for(auto it=begin; it!=end; it++) {
				auto castedAspect = dynamic_cast<CLASS*>(*it);
				if(castedAspect != nullptr) {
					if(count == INDEX) {
						aspects.insert(it, aspect);
						onAddAspect(aspect);
						return this;
					}
					count++;
				}
			}
			if(!appendIfIndexMissing) {
				throw fgl::IllegalArgumentException("INDEX", "number of aspects not in Aspectable");
			}
			aspects.push_back(aspect);
			onAddAspect(aspect);
			return this;
		}
		
		template<typename CLASS, size_t INDEX = 0>
		Aspectable* addAspectAfter(ASPECT* aspect, bool appendIfIndexMissing = false) {
			auto begin = aspects.begin();
			auto end = aspects.end();
			size_t count = 0;
			for(auto it=begin; it!=end; it++) {
				auto castedAspect = dynamic_cast<CLASS*>(*it);
				if(castedAspect != nullptr) {
					if(count == INDEX) {
						it++;
						aspects.insert(it, aspect);
						onAddAspect(aspect);
						return this;
					}
					count++;
				}
			}
			if(!appendIfIndexMissing) {
				throw fgl::IllegalArgumentException("INDEX", "number of aspects not in Aspectable");
			}
			aspects.push_back(aspect);
			onAddAspect(aspect);
			return this;
		}
		
		template<typename CLASS>
		inline CLASS* getAspect() {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS>
		inline const CLASS* getAspect() const {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS>
		inline CLASS* getAspectWhere(const std::function<bool(const CLASS*)>& func) {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(func(castedAspect)) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS>
		inline const CLASS* getAspectWhere(const std::function<bool(const CLASS*)>& func) const {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(func(castedAspect)) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS>
		inline std::list<CLASS*> getAspects() {
			std::list<CLASS*> castedAspects;
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					castedAspects.push_back(castedAspect);
				}
			}
			return castedAspects;
		}
		
		template<typename CLASS>
		inline std::list<const CLASS*> getAspects() const {
			std::list<const CLASS*> castedAspects;
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					castedAspects.push_back(castedAspect);
				}
			}
			return castedAspects;
		}
		
		inline const std::list<ASPECT*>& getAllAspects() {
			return aspects;
		}
		
		inline const std::list<const ASPECT*>& getAllAspects() const {
			return reinterpret_cast<const std::list<const ASPECT*>&>(aspects);
		}
		
	protected:
		virtual void onAddAspect(ASPECT* aspect) {
			//
		}
		
	private:
		std::list<ASPECT*> aspects;
	};
}
