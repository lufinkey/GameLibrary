
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
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline CLASS* getAspect() {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline const CLASS* getAspect() const {
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					return castedAspect;
				}
			}
			return nullptr;
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline ArrayList<CLASS*> getAspects() {
			std::list<CLASS*> castedAspects;
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					castedAspects.push_back(castedAspect);
				}
			}
			return ArrayList<CLASS*>(castedAspects);
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline ArrayList<const CLASS*> getAspects() const {
			std::list<const CLASS*> castedAspects;
			for(auto aspect : aspects) {
				auto castedAspect = dynamic_cast<CLASS*>(aspect);
				if(castedAspect != nullptr) {
					castedAspects.push_back(castedAspect);
				}
			}
			return ArrayList<CLASS*>(castedAspects);
		}
		
		inline ArrayList<ASPECT*> getAllAspects() {
			return ArrayList<ASPECT*>(aspects);
		}
		
		inline ArrayList<const ASPECT*> getAllAspects() const {
			return ArrayList<const ASPECT*>(reinterpret_cast<const std::list<const ASPECT*>&>(aspects));
		}
		
	protected:
		virtual void onAddAspect(ASPECT* aspect) {
			//
		}
		
	private:
		std::list<ASPECT*> aspects;
	};
}
