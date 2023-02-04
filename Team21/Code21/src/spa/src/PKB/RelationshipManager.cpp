//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"
#include "RelationshipHashFactory.h"


RelationshipManager::RelationshipManager() {
    this->relationshipMap = new std::unordered_map<std::string, std::vector<Relationship *> *>();
}

RelationshipManager::~RelationshipManager() {
        // todo delete all the vectors in the hash map

        delete this->relationshipMap;
}

void RelationshipManager::storeRelationship(Relationship *relationship) {

    RelationshipHashFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(relationship);

    // If the hash key does not exist, create a new vector and insert it into the map
    if (this->relationshipMap->find(hashKey) == this->relationshipMap->end()) {
        this->relationshipMap->insert({hashKey, new std::vector<Relationship *>()});
    }
    this->relationshipMap->at(hashKey)->push_back(relationship);
}

std::vector<Relationship *> *RelationshipManager::getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType, EntityType rightHandEntityType) {

    RelationshipHashFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);

    // If the hash key does not exist, create a new vector and insert it into the map
    if (this->relationshipMap->find(hashKey) == this->relationshipMap->end()) {
        this->relationshipMap->insert({hashKey, new std::vector<Relationship *>()});
    }

    return this->relationshipMap->at(hashKey);
}
