//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <iostream>
#include <memory>
#include <stdexcept>

#include "RelationshipLiteralHashkeyGenerator.h"
#include "RelationshipSynonymHashkeyGenerator.h"

RelationshipManager::RelationshipManager() {
    this->relationshipSynonymHashToRelationshipStore = std::unordered_map<int, std::shared_ptr<std::vector<Relationship *>>>();
    this->relationshipLiteralHashToRelationshipStore = std::unordered_map<std::string, std::shared_ptr<Relationship>>();
}

RelationshipManager::~RelationshipManager() {
    this->relationshipSynonymHashToRelationshipStore.clear();
}

void RelationshipManager::storeRelationship(std::shared_ptr<Relationship> relationship) {
    RelationshipLiteralHashkeyGenerator relationshipLiteralHashFactory;
    std::string literalHashkey = relationshipLiteralHashFactory.getHashKey(relationship.get());

    if (this->getRelationshipIfExist(relationship.get()) != nullptr) {
        return;
    }

    this->relationshipLiteralHashToRelationshipStore.insert({literalHashkey, std::shared_ptr<Relationship>(relationship)});
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int synonymHashkey = relationshipHashFactory.getHashKey(relationship.get());

    // Wrap the relationship in a shared_ptr and pass ownership to the vector in the map
    RelationshipManager::initialiseVectorForIndexIfNotExist(synonymHashkey);
    this->relationshipSynonymHashToRelationshipStore.at(synonymHashkey)->push_back(relationship.get());
}

std::vector<Relationship *> *
RelationshipManager::getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType,
                                             EntityType rightHandEntityType) {
    int hashKey = RelationshipSynonymHashkeyGenerator().getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);

    if (this->relationshipSynonymHashToRelationshipStore.find(hashKey) == this->relationshipSynonymHashToRelationshipStore.end()) {
        auto derivedHashKeys = RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(
            relationshipType, leftHandEntityType, rightHandEntityType);

        auto relationships = std::make_shared<std::vector<Relationship *>>();

        for (int derivedHashKey : *derivedHashKeys) {
            initialiseVectorForIndexIfNotExist(derivedHashKey);
            auto relationshipsForHashKey = this->relationshipSynonymHashToRelationshipStore.at(derivedHashKey);
            relationships->insert(relationships->end(), relationshipsForHashKey->begin(), relationshipsForHashKey->end());
        }

        // Memoise the relationships in the map for future use, and also to take ownership of the vector
        this->relationshipSynonymHashToRelationshipStore.insert({hashKey, relationships});
    }

    return this->relationshipSynonymHashToRelationshipStore.at(hashKey).get();
}

Relationship *RelationshipManager::getRelationshipIfExist(Relationship *relationship) {
    RelationshipLiteralHashkeyGenerator relationshipLiteralHashFactory;
    std::string literalHashkey = relationshipLiteralHashFactory.getHashKey(relationship);

    if (this->relationshipLiteralHashToRelationshipStore.find(literalHashkey) == this->relationshipLiteralHashToRelationshipStore.end()) {
        return nullptr;
    }
    return this->relationshipLiteralHashToRelationshipStore.at(literalHashkey).get();
}

void RelationshipManager::initialiseVectorForIndexIfNotExist(int hashkey) {
    if (this->relationshipSynonymHashToRelationshipStore.find(hashkey) == this->relationshipSynonymHashToRelationshipStore.end()) {
        this->relationshipSynonymHashToRelationshipStore.insert({hashkey, std::make_shared<std::vector<Relationship *>>()});
    }
}

std::shared_ptr<std::vector<int>> RelationshipManager::getPossibleHashKeysForGivenEntityAndRelationshipTypes(RelationshipType relationshipType,
                                                                                                             EntityType leftHandEntityType,
                                                                                                             EntityType rightHandEntityType) {
    std::shared_ptr<std::vector<int>> hashKeys = std::make_shared<std::vector<int>>();

    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
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
