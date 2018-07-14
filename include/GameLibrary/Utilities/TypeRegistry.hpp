
#pragma once

#include <typeinfo>
#include <list>
#include <map>
#include <vector>

namespace fgl
{
	typedef size_t TypeRegistryId;
	
	template<typename BASE_CLASS>
	using ObjectRegistry = std::map<TypeRegistryId, std::list<BASE_CLASS*>>;
	
	
	
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
		TypeRegistryId registerType(const std::type_info& typeInfo, const std::vector<TypeRegistryId>& parentTypeRegistryIds);
		template<typename CLASS, typename... PARENT_CLASS>
		inline TypeRegistryId registerType() {
			// validate parent classes
			static_assert(all_true<(std::is_base_of<PARENT_CLASS, CLASS>::value)...>::value,
				"CLASS template argument must be derived from PARENT_CLASS");
			// register type
			return registerType(typeid(CLASS), std::vector<TypeRegistryId>(getTypeRegistryId<PARENT_CLASS>()...));
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
		std::vector<BASE_CLASS*> findTypes(const ObjectRegistry<BASE_CLASS*>& objectRegistry, TypeRegistryId typeRegistryId, bool firstOnly=false) {
			std::vector<BASE_CLASS*> types;
			try {
				auto&& objects = objectRegistry.at(typeRegistryId);
				if(firstOnly && objects.size() > 0) {
					return std::vector<BASE_CLASS*>({*objects.begin()});
				}
				types.insert(types.end(), std::make_move_iterator(objects.begin()), std::make_move_iterator(objects.end()));
			}
			catch(const std::out_of_range&) {
				//
			}
			auto derivedTypes = getDerivedTypes<BASE_CLASS>();
			for(auto& derivedTypeRegistryId : derivedTypes) {
				try {
					auto&& objects = objectRegistry.at(typeRegistryId);
					if(firstOnly && objects.size() > 0) {
						return std::vector<BASE_CLASS*>({*objects.begin()});
					}
					types.insert(types.end(), std::make_move_iterator(objects.begin()), std::make_move_iterator(objects.end()));
				}
				catch(const std::out_of_range&) {
					//
				}
			}
			return types;
		}
		
		template<typename CLASS, typename BASE_CLASS>
		inline std::vector<CLASS*> findTypes(const ObjectRegistry<BASE_CLASS*>& objectRegistry, bool firstOnly=false) {
			return std::vector<CLASS*>(findTypes<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>(), firstOnly));
		}
		
		template<typename BASE_CLASS>
		inline BASE_CLASS* findType(const ObjectRegistry<BASE_CLASS*>& objectRegistry, TypeRegistryId typeRegistryId) {
			auto&& types = findTypes<BASE_CLASS>(objectRegistry, typeRegistryId, true);
			if(types.size() == 0) {
				return nullptr;
			}
			return types[0];
		}
		
		template<typename CLASS, typename BASE_CLASS>
		inline CLASS* findType(const ObjectRegistry<BASE_CLASS*>& objectRegistry) {
			return static_cast<CLASS*>(findType<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>()));
		}
		
	private:
		void updateDerivedTypes(TypeRegistryId typeRegistryId, TypeRegistryId derivedTypeRegistryId);
		
		// variadic true/false values
		template<bool...>
		struct bool_pack;
		template<bool... bs>
		using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;
		
		std::map<TypeRegistryId, std::list<TypeRegistryId>> derivedTypes;
		std::map<TypeRegistryId, std::list<TypeRegistryId>> parentTypes;
	};
	
	
	
	TypeRegistry globalTypeRegistry = TypeRegistry();
	
	#define REGISTER_TYPE(CLASS, ...) namespace __typeregistry { const TypeRegistryId type_##CLASS = ::fgl::globalTypeRegistry.registerType<CLASS, ##__VA_ARGS__ >(); }
	#define REGISTER_NAMESPACED_TYPE(NAMESPACE, ...) namespace NAMESPACE { REGISTER_TYPE(__VA_ARGS__) }
}
