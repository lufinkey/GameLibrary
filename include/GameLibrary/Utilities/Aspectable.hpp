
#pragma once

#include "ArrayList.hpp"
#include "TypeRegistry.hpp"

namespace fgl
{
	class Aspect
	{
	public:
		virtual ~Aspect() = default;
		
		virtual bool getFlag(const String& flag) const { return false; }
	};
	
	
	
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
				iter->push_back(aspect);
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
		
	protected:
		virtual void onAddAspect(TypeRegistryId aspectType, ASPECT* aspect) {
			//
		}
		
	private:
		std::map<TypeRegistryId, std::list<ASPECT*>> aspects;
	};
}

REGISTER_TYPE(fgl::Aspect)
