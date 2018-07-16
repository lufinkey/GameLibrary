
#pragma once

#include "ArrayList.hpp"
#include "TypeRegistry.hpp"

namespace fgl
{
	#define ENABLE_IF_EXTENDS(BASE_CLASS, CLASS) typename std::enable_if<std::is_base_of<BASE_CLASS, CLASS>::value, std::nullptr_t>::type = nullptr
	
	template<typename ASPECT=Aspect>
	class Aspectable
	{
	public:
		using AspectType = ASPECT;
		
		Aspectable() {
			//
		}
		
		virtual ~Aspectable() {
			for(auto& pair : aspects) {
				for(auto aspect : pair.second) {
					delete aspect;
				}
			}
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		Aspectable* addAspect(CLASS* aspect) {
			auto typeRegistryId = getTypeRegistryId<CLASS>();
			auto iter = aspects.find(typeRegistryId);
			if(iter == aspects.end()) {
				aspects[typeRegistryId] = { aspect };
			}
			else {
				iter->second.push_back(aspect);
			}
			onAddAspect(typeRegistryId, aspect);
			return this;
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline CLASS* getAspect() {
			return TypeRegistry::findType<CLASS, _ASPECT>(aspects);
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline const CLASS* getAspect() const {
			return TypeRegistry::findType<CLASS, _ASPECT>(aspects);
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline ArrayList<CLASS*> getAspects() {
			return TypeRegistry::findTypes<CLASS, _ASPECT>(aspects);
		}
		
		template<typename CLASS, typename _ASPECT=ASPECT, ENABLE_IF_EXTENDS(_ASPECT, CLASS)>
		inline ArrayList<const CLASS*> getAspects() const {
			return TypeRegistry::findTypes<CLASS, _ASPECT>(aspects);
		}
		
		inline ArrayList<ASPECT*> getAllAspects() {
			size_t aspectCount = 0;
			for(auto& pair : aspects) {
				aspectCount += pair.second.size();
			}
			fgl::ArrayList<ASPECT*> allAspects;
			allAspects.reserve(aspectCount);
			for(auto& pair : aspects) {
				allAspects.addAll(fgl::ArrayList<ASPECT*>(pair.second));
			}
			return allAspects;
		}
		
		inline ArrayList<const ASPECT*> getAllAspects() const {
			size_t aspectCount = 0;
			for(auto& pair : aspects) {
				aspectCount += pair.second.size();
			}
			fgl::ArrayList<const ASPECT*> allAspects;
			allAspects.reserve(aspectCount);
			for(auto& pair : aspects) {
				allAspects.addAll(fgl::ArrayList<ASPECT*>(pair.second));
			}
			return allAspects;
		}
		
	protected:
		virtual void onAddAspect(TypeRegistryId aspectType, ASPECT* aspect) {
			//
		}
		
	private:
		std::map<TypeRegistryId, std::list<ASPECT*>> aspects;
	};
}
