
#pragma once

#include <typeinfo>
#include <typeindex>
#include <list>
#include <map>
#include <string>
#include <vector>

//#define DISABLE_TYPE_REGISTRY

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
	struct TypeInfo;
	
	template<typename... CLASS>
	using ClassList = std::tuple<TypeInfo<CLASS>...>;
	
	template<typename... T>
	struct TypeInfo {};
	
	template<>
	struct TypeInfo<> {
		template<typename U>
		static bool derive() {
			return true;
		}
	};
	
	
	
	class TypeRegistry
	{
		template<typename... T>
		friend struct TypeInfo;
	private:
		TypeRegistry() = delete;
		
		// derive a registered type
		template<typename CLASS, typename PARENT_CLASS>
		static inline bool deriveType() {
			static_assert(std::is_base_of<PARENT_CLASS, CLASS>::value, "CLASS template argument must be derived from PARENT_CLASS"); \
			return TypeInfo<PARENT_CLASS>::template derive<CLASS>();
		}
		
		// derive registered types
		template<typename CLASS, typename... PARENT_CLASS>
		static inline bool deriveTypes() {
			auto results = std::vector<bool>(deriveType<CLASS, PARENT_CLASS>()...);
			for(auto result : results) {
				if(!result) {
					return false;
				}
			}
			return true;
		}
		
		template<typename CLASS, typename BASE_CLASS>
		static std::vector<CLASS*> castVector(const std::vector<BASE_CLASS*>& vect) {
			std::vector<CLASS*> newVect;
			newVect.reserve(vect.size());
			for(auto obj : vect) {
				newVect.push_back(static_cast<CLASS*>(obj));
			}
			return newVect;
		}
		
	public:
		// find registered types
		template<typename BASE_CLASS>
		static std::vector<BASE_CLASS*> findTypes(const ObjectRegistry<BASE_CLASS>& objectRegistry, TypeRegistryId typeRegistryId, bool firstOnly=false) {
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
			#ifndef DISABLE_TYPE_REGISTRY
				auto& derivedTypes = TypeInfo<BASE_CLASS>::derivedList();
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
			#endif
			return types;
		}
		
		// find registered types
		template<typename CLASS, typename BASE_CLASS>
		static inline std::vector<CLASS*> findTypes(const ObjectRegistry<BASE_CLASS>& objectRegistry, bool firstOnly=false) {
			return castVector<CLASS, BASE_CLASS>(
				findTypes<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>(), firstOnly)
			);
		}
		
		// find a registered type
		template<typename BASE_CLASS>
		static inline BASE_CLASS* findType(const ObjectRegistry<BASE_CLASS>& objectRegistry, TypeRegistryId typeRegistryId) {
			auto&& types = findTypes<BASE_CLASS>(objectRegistry, typeRegistryId, true);
			if(types.size() == 0) {
				return nullptr;
			}
			return types[0];
		}
		
		// find a registered type
		template<typename CLASS, typename BASE_CLASS>
		static inline CLASS* findType(const ObjectRegistry<BASE_CLASS>& objectRegistry) {
			return static_cast<CLASS*>(findType<BASE_CLASS>(objectRegistry, getTypeRegistryId<CLASS>()));
		}
	};
}


#ifdef DISABLE_TYPE_REGISTRY
	#define REGISTER_TYPE(...)
#else
	#define REGISTER_TYPE(CLASS, ...) \
		namespace fgl { \
			template<> \
			struct TypeInfo<CLASS> { \
				friend class TypeRegistry; \
				friend class InnerRegister; \
				using parentRegistrations = ClassList<__VA_ARGS__>; \
				static TypeRegistryId id() { \
					return getTypeRegistryId<CLASS>(); \
				} \
				static std::string name() { \
					return #CLASS; \
				} \
				static std::string mangledName() { \
					return typeid(CLASS).name(); \
				} \
				static std::vector<TypeRegistryId> parents() { \
					return getTypeRegistryIds<__VA_ARGS__>(); \
				} \
				static std::vector<TypeRegistryId> derived() { \
					registerType(); \
					auto& _derived = derivedList(); \
					return std::vector<TypeRegistryId>(_derived.begin(), _derived.end()); \
				} \
				\
			private: \
				static fgl::TypeRegistryId registerType() { \
					static bool registered = false; \
					auto typeId = getTypeRegistryId<CLASS>(); \
					if(registered) { \
						return typeId; \
					} \
					registered = true; \
					TypeRegistry::template deriveTypes<CLASS, ##__VA_ARGS__>(); \
					return typeId; \
				} \
				template<typename DERIVED_CLASS> \
				static bool derive() { \
					auto typeRegistryId = getTypeRegistryId<DERIVED_CLASS>(); \
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
		}
#endif
