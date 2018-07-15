
#pragma once

#include <typeinfo>
#include <typeindex>
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
	template<typename... CLASS>
	inline std::vector<TypeRegistryId> getTypeRegistryIds() {
		return std::vector<TypeRegistryId>(getTypeRegistryId<CLASS>()...);
	}
	
	
	
	template<typename... T>
	struct TypeRegistration;
	
	template<typename... CLASS>
	using ClassList = std::tuple<TypeRegistration<CLASS>...>;
	
	template<typename... T>
	struct TypeRegistration {};
	
	template<>
	struct TypeRegistration<> {
		template<typename U>
		static bool derive() {
			return true;
		}
	};
	
	
	
	class TypeRegistry
	{
	private:
		// variadic true/false values
		template<bool...>
		struct bool_pack;
		template<bool... bs>
		using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;
		
	public:
		TypeRegistry() = delete;
		
		// register a type
		template<typename CLASS, typename... PARENT_CLASS>
		static inline TypeRegistryId registerType() {
			// validate parent classes
			static_assert(all_true<(std::is_base_of<PARENT_CLASS, CLASS>::value)...>::value,
				"CLASS template argument must be derived from PARENT_CLASS");
			// register type
			return deriveTypes<CLASS, PARENT_CLASS...>();
		}
		
		// find registered types
		template<typename BASE_CLASS>
		static std::vector<BASE_CLASS*> findTypes(const ObjectRegistry<BASE_CLASS*>& objectRegistry, TypeRegistryId typeRegistryId, bool firstOnly=false) {
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
			auto& derivedTypes = TypeRegistration<BASE_CLASS>::derivedList();
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
		
		// find registered types
		template<typename CLASS, typename BASE_CLASS>
		static inline std::vector<CLASS*> findTypes(const ObjectRegistry<BASE_CLASS*>& objectRegistry, bool firstOnly=false) {
			return std::vector<CLASS*>(findTypes<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>(), firstOnly));
		}
		
		// find a registered type
		template<typename BASE_CLASS>
		static inline BASE_CLASS* findType(const ObjectRegistry<BASE_CLASS*>& objectRegistry, TypeRegistryId typeRegistryId) {
			auto&& types = findTypes<BASE_CLASS>(objectRegistry, typeRegistryId, true);
			if(types.size() == 0) {
				return nullptr;
			}
			return types[0];
		}
		
		// find a registered type
		template<typename CLASS, typename BASE_CLASS>
		static inline CLASS* findType(const ObjectRegistry<BASE_CLASS*>& objectRegistry) {
			return static_cast<CLASS*>(findType<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>()));
		}
		
		// derive a registered type
		template<typename DERIVED_CLASS, typename PARENT_CLASS>
		static inline bool deriveType() {
			return TypeRegistration<PARENT_CLASS>::template derive<DERIVED_CLASS>();
		}
		
		// derive registered types
		template<typename DERIVED_CLASS, typename... PARENT_CLASS>
		static inline bool deriveTypes() {
			auto results = std::vector<bool>(deriveType<DERIVED_CLASS, PARENT_CLASS>()...);
			for(auto result : results) {
				if(!result) {
					return false;
				}
			}
			return true;
		}
	};
	
	
	
	#define REGISTER_TYPE(CLASS, ...) \
		template<> \
		struct ::fgl::TypeRegistration<CLASS> { \
			friend class TypeRegistry; \
			using parentRegistrations = ClassList<__VA_ARGS__>; \
			static TypeRegistryId id() { \
				return typeid(CLASS).hash_code(); \
			} \
			static std::vector<TypeRegistryId> parents() { \
				return getTypeRegistryIds<__VA_ARGS__>(); \
			} \
			static std::vector<TypeRegistryId> derived() { \
				auto& _derived = derivedList(); \
				return std::vector<TypeRegistryId>(_derived.begin(), _derived.end()); \
			} \
			\
		private: \
			template<typename DERIVED_CLASS> \
			static bool derive() { \
				TypeRegistryId typeRegistryId = getTypeRegistryId<DERIVED_CLASS>(); \
				auto& _derived = derivedList(); \
				for(auto& cmpType : _derived) { \
					if(cmpType == typeRegistryId) { \
						return false; \
					} \
				} \
				_derived.push_back(typeRegistryId); \
				return TypeRegistry::deriveTypes<DERIVED_CLASS, ##__VA_ARGS__>(); \
			} \
			static std::list<TypeRegistryId>& derivedList() { \
				static std::list<TypeRegistryId> _derived; \
				return _derived; \
			} \
		}; \
		namespace __typeregistry { \
			const TypeRegistryId type_##CLASS = ::fgl::TypeRegistry::registerType<CLASS, ##__VA_ARGS__ >(); \
		}
	#define REGISTER_NAMESPACED_TYPE(NAMESPACE, ...) \
		namespace NAMESPACE { \
			REGISTER_TYPE(__VA_ARGS__) \
		}
}
