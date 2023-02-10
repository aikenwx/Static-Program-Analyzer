//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <memory>

#include "RelationshipHashkeyFactory.h"

RelationshipManager::RelationshipManager() {
    this->relationshipMap = std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<Relationship>>>>();
}

RelationshipManager::~RelationshipManager() {
    this->relationshipMap.clear();
}

void RelationshipManager::storeRelationship(Relationship *relationship) {
    RelationshipHashkeyFactory relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(relationship);

    // Wrap the relationship in a shared_ptr and pass ownership to the vector in the map
    std::shared_ptr<Relationship> relationshipPtr = std::shared_ptr<Relationship>(relationship);
    RelationshipManager::initialiseVectorForIndexIfNotExist(hashKey);
    this->relationshipMap.at(hashKey)->push_back(relationshipPtr);
}

std::vector<std::shared_ptr<Relationship>> *
RelationshipManager::getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType,
                                             EntityType rightHandEntityType) {
    int hashKey = RelationshipHashkeyFactory().getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);

    if (this->relationshipMap.find(hashKey) == this->relationshipMap.end()) {
        auto derivedHashKeys = RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(
            relationshipType, leftHandEntityType, rightHandEntityType);
        auto relationships = std::make_shared<std::vector<std::shared_ptr<Relationship>>>();

        for (int derivedHashKey : *derivedHashKeys) {
            initialiseVectorForIndexIfNotExist(derivedHashKey);
            auto relationshipsForHashKey = this->relationshipMap.at(derivedHashKey);
            relationships->insert(relationships->end(), relationshipsForHashKey->begin(), relationshipsForHashKey->end());
        }

        // Memoise the relationships in the map for future use, and also to take ownership of the vector
        this->relationshipMap.insert({hashKey, relationships});
    }

    return this->relationshipMap.at(hashKey).get();
}

void RelationshipManager::initialiseVectorForIndexIfNotExist(int hashkey) {
    if (this->relationshipMap.find(hashkey) == this->relationshipMap.end()) {
        this->relationshipMap.insert({hashkey, std::make_shared<std::vector<std::shared_ptr<Relationship>>>()});
    }
}

std::shared_ptr<std::vector<int>> RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(RelationshipType relationshipType,
                                                                                                             EntityType leftHandEntityType,
                                                                                                             EntityType rightHandEntityType) {
    std::shared_ptr<std::vector<int>> hashKeys = std::make_shared<std::vector<int>>();

    RelationshipHashkeyFactory relationshipHashFactory;
    std::vector<EntityType> possibleLeftEntityTypes = {leftHandEntityType};
    std::vector<EntityType> possibleRightEntityTypes = {rightHandEntityType};

    if (leftHandEntityType == EntityType::STATEMENT) {
        possibleLeftEntityTypes = Entity::statementTypes;
    }

    if (rightHandEntityType == EntityType::STATEMENT) {
        possibleRightEntityTypes = Entity::statementTypes;
    }

    for (EntityType leftEntityType : possibleLeftEntityTypes) {
        for (EntityType rightEntityType : possibleRightEntityTypes) {
            auto hashKey = relationshipHashFactory.getHashKey(relationshipType, leftEntityType, rightEntityType);
            hashKeys->push_back(hashKey);
        }
    }
    return hashKeys;
}
