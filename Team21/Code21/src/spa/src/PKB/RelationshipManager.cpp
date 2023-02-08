//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"
#include "RelationshipHashkeyFactory.h"

RelationshipManager::RelationshipManager() {
    this->relationshipMap = new std::unordered_map<std::string, std::vector<Relationship *> *>();
}

RelationshipManager::~RelationshipManager() {
    // todo delete all the vectors in the hash map

    delete this->relationshipMap;
}

void RelationshipManager::storeRelationship(Relationship *relationship) {

    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(relationship);

    RelationshipManager::initialiseVectorForIndexIfNotExist(hashKey);
    this->relationshipMap->at(hashKey)->push_back(relationship);
}

std::vector<Relationship *> *
RelationshipManager::getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType,
                                             EntityType rightHandEntityType) {

    std::vector<std::string> *hashKeys = RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(
            relationshipType, leftHandEntityType, rightHandEntityType);

    // get all relationships
    std::vector<Relationship *> *relationships = new std::vector<Relationship *>();
    for (std::string hashKey: *hashKeys) {
        initialiseVectorForIndexIfNotExist(hashKey);
        std::vector<Relationship *> *relationshipsForHashKey = this->relationshipMap->at(hashKey);

        relationships->insert(relationships->end(), relationshipsForHashKey->begin(), relationshipsForHashKey->end());
    }
    delete hashKeys;
    return relationships;
}

void RelationshipManager::initialiseVectorForIndexIfNotExist(std::string hashkey) {
    if (this->relationshipMap->find(hashkey) == this->relationshipMap->end()) {
        this->relationshipMap->insert({hashkey, new std::vector<Relationship *>()});
    }
}

std::vector<std::string> *RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(RelationshipType relationshipType,
                                                                           EntityType leftHandEntityType,
                                                                           EntityType rightHandEntityType) {
    std::vector<std::string> *hashKeys = new std::vector<std::string>();



    RelationshipHashkeyFactory relationshipHashFactory;
    std::vector<EntityType> possibleLeftEntityTypes = {leftHandEntityType};
    std::vector<EntityType> possibleRightEntityTypes = {rightHandEntityType};

    if (leftHandEntityType == EntityType::STATEMENT) {
        possibleLeftEntityTypes = Entity::statementTypes;
    }

    if (rightHandEntityType == EntityType::STATEMENT) {
        possibleRightEntityTypes = Entity::statementTypes;
    }

    for (EntityType leftEntityType: possibleLeftEntityTypes) {
        for (EntityType rightEntityType: possibleRightEntityTypes) {
            std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftEntityType, rightEntityType);
            hashKeys->push_back(hashKey);
        }
    }
    return hashKeys;
}
