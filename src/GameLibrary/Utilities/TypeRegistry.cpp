
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	void TypeRegistry::updateDerivedTypes(TypeRegistryId typeRegistryId, TypeRegistryId derivedTypeRegistryId) {
		auto iter = derivedTypes.find(typeRegistryId);
		if(iter == derivedTypes.end()) {
			derivedTypes[typeRegistryId] = { derivedTypeRegistryId };
		}
		else {
			iter->second.push_back(derivedTypeRegistryId);
		}
		TypeRegistryId parentTypeRegistryId;
		try {
			parentTypeRegistryId = parentTypes.at(typeRegistryId);
		}
		catch(const std::out_of_range&) {
			return;
		}
		if(parentTypeRegistryId != getTypeRegistryId<void>()) {
			updateDerivedTypes(parentTypeRegistryId, derivedTypeRegistryId);
		}
	}
	
	TypeRegistryId TypeRegistry::registerType(const std::type_info& typeInfo, const std::type_info& parentTypeInfo) {
		auto typeRegistryId = getTypeRegistryId(typeInfo);
		auto parentTypeRegistryId = getTypeRegistryId(parentTypeInfo);
		if(parentTypes.find(typeRegistryId) != parentTypes.end()) {
			throw IllegalStateException((String)typeInfo.name() + " already registered");
		}
		parentTypes[typeRegistryId] = parentTypeRegistryId;
		if(parentTypeRegistryId != getTypeRegistryId<void>()) {
			updateDerivedTypes(parentTypeRegistryId, typeRegistryId);
		}
		return typeRegistryId;
	}
	
	std::list<TypeRegistryId> TypeRegistry::getDerivedTypes(TypeRegistryId typeRegistryId) {
		try {
			return derivedTypes.at(typeRegistryId);
		}
		catch(const std::out_of_range&) {
			return std::list<TypeRegistryId>();
		}
	}
	
	TypeRegistryId TypeRegistry::getParentType(TypeRegistryId typeRegistryId) {
		try {
			return parentTypes.at(typeRegistryId);
		}
		catch(const std::out_of_range&) {
			return getTypeRegistryId<void>();
		}
	}
}
