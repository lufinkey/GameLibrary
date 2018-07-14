
#pragma once

#include "ArrayList.hpp"
#include <typeinfo>
#include <list>
#include <map>

namespace fgl
{
	typedef size_t TypeRegistryId;
	
	
	
	inline TypeRegistryId getTypeRegistryId(const std::type_info& typeInfo) {
		return typeInfo.hash_code();
	}
	template<typename CLASS>
	inline TypeRegistryId getTypeRegistryId() {
		return getTypeRegistryId(typeid(CLASS));
	}
	
	
	
	class TypeRegistry
	{
	public:
		TypeRegistryId registerType(const std::type_info& typeInfo, const ArrayList<TypeRegistryId>& parentTypeRegistryIds);
		template<typename CLASS, typename... PARENT_CLASS>
		inline TypeRegistryId registerType() {
			return registerType(typeid(CLASS), ArrayList<TypeRegistryId>(getTypeRegistryId<PARENT_CLASS>()...));
		}
		
		std::list<TypeRegistryId> getDerivedTypes(TypeRegistryId typeRegistryId);
		
		template<typename BASE_CLASS>
		inline std::list<TypeRegistryId> getDerivedTypes() {
			return getDerivedTypes(getTypeRegistryId<BASE_CLASS>());
		}
		
		std::list<TypeRegistryId> getParentTypes(TypeRegistryId typeRegistryId);
		
		template<typename BASE_CLASS>
		inline std::list<TypeRegistryId> getParentTypes() {
			return getParentTypes(getTypeRegistryId<BASE_CLASS>());
		}
		
		template<typename BASE_CLASS>
		ArrayList<BASE_CLASS*> findTypes(const std::map<TypeRegistryId,std::list<BASE_CLASS*>>& objectRegistry, TypeRegistryId typeRegistryId, bool firstOnly=false) {
			fgl::ArrayList<BASE_CLASS*> types;
			try {
				auto&& objects = objectRegistry.at(typeRegistryId);
				if(firstOnly && objects.size() > 0) {
					return fgl::ArrayList<BASE_CLASS*>({*objects.begin()});
				}
				types.addAll(objects);
			}
			catch(const std::out_of_range&) {
				//
			}
			auto derivedTypes = getDerivedTypes<BASE_CLASS>();
			for(auto& derivedTypeRegistryId : derivedTypes) {
				try {
					auto&& objects = objectRegistry.at(typeRegistryId);
					if(firstOnly && objects.size() > 0) {
						return fgl::ArrayList<BASE_CLASS*>({*objects.begin()});
					}
					types.addAll(objects);
				}
				catch(const std::out_of_range&) {
					//
				}
			}
			return types;
		}
		
		template<typename CLASS, typename BASE_CLASS>
		inline ArrayList<CLASS*> findTypes(const std::map<TypeRegistryId,std::list<BASE_CLASS*>>& objectRegistry, bool firstOnly=false) {
			return ArrayList<CLASS*>(findTypes<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>(), firstOnly));
		}
		
		template<typename BASE_CLASS>
		inline BASE_CLASS* findType(const std::map<TypeRegistryId,std::list<BASE_CLASS*>>& objectRegistry, TypeRegistryId typeRegistryId) {
			auto&& types = findTypes<BASE_CLASS>(objectRegistry, typeRegistryId, true);
			if(types.size() == 0) {
				return nullptr;
			}
			return types[0];
		}
		
		template<typename CLASS, typename BASE_CLASS>
		inline CLASS* findType(const std::map<TypeRegistryId,std::list<BASE_CLASS*>>& objectRegistry) {
			return static_cast<CLASS*>(findType<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>()));
		}
		
	private:
		void updateDerivedTypes(TypeRegistryId typeRegistryId, TypeRegistryId derivedTypeRegistryId);
		
		std::map<TypeRegistryId, std::list<TypeRegistryId>> derivedTypes;
		std::map<TypeRegistryId, std::list<TypeRegistryId>> parentTypes;
	};
	
	
	
	TypeRegistry globalTypeRegistry = TypeRegistry();
	
	#define REGISTER_TYPE(CLASS, ...) namespace __typeregistry { const TypeRegistryId type_##CLASS = globalTypeRegistry.registerType<CLASS, ##__VA_ARGS__ >(); }
	#define REGISTER_NAMESPACED_TYPE(NAMESPACE, ...) namespace NAMESPACE { REGISTER_TYPE(__VA_ARGS__) }
}
