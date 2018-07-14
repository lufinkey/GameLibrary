
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
		std::list<TypeRegistryId> parentTypeRegistryIds;
		try {
			parentTypeRegistryIds = parentTypes.at(typeRegistryId);
		}
		catch(const std::out_of_range&) {
			return;
		}
		for(auto parentTypeRegistryId : parentTypeRegistryIds) {
			updateDerivedTypes(parentTypeRegistryId, derivedTypeRegistryId);
		}
	}
	
	TypeRegistryId TypeRegistry::registerType(const std::type_info& typeInfo, const ArrayList<TypeRegistryId>& parentTypeRegistryIds) {
		auto typeRegistryId = getTypeRegistryId(typeInfo);
		if(parentTypes.find(typeRegistryId) != parentTypes.end()) {
			throw IllegalStateException((String)typeInfo.name() + " already registered");
		}
		for(auto parentTypeRegistryId : parentTypeRegistryIds) {
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
	
	std::list<TypeRegistryId> TypeRegistry::getParentTypes(TypeRegistryId typeRegistryId) {
		try {
			return parentTypes.at(typeRegistryId);
		}
		catch(const std::out_of_range&) {
			return std::list<TypeRegistryId>();
		}
	}
}
