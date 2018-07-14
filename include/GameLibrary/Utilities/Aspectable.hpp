
#pragma once

#include "ArrayList.hpp"
#include "Retainable.hpp"
#include "TypeRegistry.hpp"

namespace fgl
{
	class Aspect : public Retainable
	{
	public:
		virtual ~Aspect() = default;
	};
	
	REGISTER_TYPE(Aspect, Retainable)
	
	
	
	#define ENABLE_IF_EXTENDS_ASPECTS(CLASS) typename std::enable_if<std::is_base_of<Aspect, CLASS>::value, std::nullptr_t>::type = nullptr
	
	class Aspectable
	{
	public:
		virtual ~Aspectable() = default;
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECTS(CLASS)>
		void addAspect(CLASS* aspect) {
			auto typeRegistryId = getTypeRegistryId<CLASS>();
			auto iter = aspects.find(typeRegistryId);
			if(iter == aspects.end()) {
				aspects[typeRegistryId] = { aspect };
			}
			else {
				iter->push_back(aspect);
			}
		}
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECTS(CLASS)>
		inline CLASS* getAspect() const {
			return TypeRegistry::global()->findType<CLASS, Aspect>(aspects);
		}
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECTS(CLASS)>
		inline ArrayList<CLASS*> getAspects() const {
			return TypeRegistry::global()->findTypes<CLASS, Aspect>(aspects);
		}

	private:
		std::map<TypeRegistryId, std::list<Aspect*>> aspects;
	};
	
	REGISTER_TYPE(Aspectable)
}
