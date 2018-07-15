
#pragma once

#include "ArrayList.hpp"
#include "TypeRegistry.hpp"

namespace fgl
{
	class Aspect
	{
	public:
		virtual ~Aspect() = default;
		
		virtual bool getFlag(const String& flag) const;
	};
	
	REGISTER_TYPE(Aspect)
	
	
	
	#define ENABLE_IF_EXTENDS_ASPECT(CLASS) typename std::enable_if<std::is_base_of<Aspect, CLASS>::value, std::nullptr_t>::type = nullptr
	
	class Aspectable
	{
	public:
		Aspectable() {
			#ifdef DISABLE_GLOBAL_TYPE_REGISTRY
				static_assert(false, "Cannot use Aspectable with macro DISABLE_GLOBAL_TYPE_REGISTRY");
			#endif
		}
		
		virtual ~Aspectable();
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECT(CLASS)>
		void addAspect(CLASS* aspect) {
			auto typeRegistryId = getTypeRegistryId<CLASS>();
			auto iter = aspects.find(typeRegistryId);
			if(iter == aspects.end()) {
				aspects[typeRegistryId] = { aspect };
			}
			else {
				iter->push_back(aspect);
			}
			onAddAspect(typeRegistryId, aspect);
		}
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECT(CLASS)>
		inline CLASS* getAspect() const {
			return TypeRegistry::global()->findType<CLASS, Aspect>(aspects);
		}
		
		template<typename CLASS, ENABLE_IF_EXTENDS_ASPECT(CLASS)>
		inline ArrayList<CLASS*> getAspects() const {
			return TypeRegistry::global()->findTypes<CLASS, Aspect>(aspects);
		}
		
	protected:
		virtual void onAddAspect(TypeRegistryId aspectType, Aspect* aspect);
		
	private:
		std::map<TypeRegistryId, std::list<Aspect*>> aspects;
	};
	
	REGISTER_TYPE(Aspectable)
}
