//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <iostream>
#include <memory>
#include <stdexcept>

#include "StorageHelpers.h"

RelationshipManager::RelationshipManager() {
    this->relationshipSynonymStore = std::unordered_map<RelationshipSynonymKey, std::shared_ptr<std::vector<Relationship *>>>();
    this->relationshipStore = std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>>();
}

RelationshipManager::~RelationshipManager() {
    this->relationshipSynonymStore.clear();
}

void RelationshipManager::storeRelationship(Relationship *relationship) {
    if (relationshipStore.find(relationship->getRelationshipKey()) != relationshipStore.end()) {
        return;
    }

    this->relationshipStore.insert(std::make_pair(relationship->getRelationshipKey(), std::shared_ptr<Relationship>(relationship)));

    this->storeInRelationshipSynonymStore(relationship);
}

Relationship *RelationshipManager::getRelationship(RelationshipKey &key) {
    if (relationshipStore.find(key) != relationshipStore.end()) {
        return this->relationshipStore.at(key).get();
    }

    return nullptr;
}

std::vector<Relationship *> *
RelationshipManager::getRelationshipsByTypes(RelationshipType &relationshipType, EntityType &leftHandEntityType,
                                             EntityType &rightHandEntityType) {
    RelationshipSynonymKey relationshipSynonymKey = RelationshipSynonymKey(&relationshipType, &leftHandEntityType, &rightHandEntityType);
    this->initialiseVectorForRelationshipSynonymStoreIfNotExist(relationshipSynonymKey);

    return this->relationshipSynonymStore.at(relationshipSynonymKey).get();
}

void RelationshipManager::initialiseVectorForRelationshipSynonymStoreIfNotExist(
    RelationshipSynonymKey relationshipSynonymKey) {
    if (this->relationshipSynonymStore.find(relationshipSynonymKey) == this->relationshipSynonymStore.end()) {
        this->relationshipSynonymStore.insert({relationshipSynonymKey, std::make_shared<std::vector<Relationship *>>()});
    }
}

void RelationshipManager::storeInRelationshipSynonymStore(Relationship *relationship) {
    RelationshipSynonymKey &relationshipSynonymKey = relationship->getRelationshipSynonymKey();
    this->initialiseVectorForRelationshipSynonymStoreIfNotExist(relationshipSynonymKey);

    this->relationshipSynonymStore.at(relationshipSynonymKey)->push_back(relationship);

    if (Statement::isStatement(relationship->getLeftHandEntity())) {
        RelationshipSynonymKey leftStatementRelationship = RelationshipSynonymKey(&relationship->getRelationshipType(),
                                                                                  &Statement::getEntityTypeStatic(),
                                                                                  &relationship->getRightHandEntity()->getEntityType());
        this->initialiseVectorForRelationshipSynonymStoreIfNotExist(leftStatementRelationship);

        this->relationshipSynonymStore.at(leftStatementRelationship)->push_back(relationship);
    }
    if (Statement::isStatement(relationship->getRightHandEntity())) {
        RelationshipSynonymKey rightStatementRelationship = RelationshipSynonymKey(&relationship->getRelationshipType(),
                                                                                   &relationship->getLeftHandEntity()->getEntityType(),
                                                                                   &Statement::getEntityTypeStatic());
        this->initialiseVectorForRelationshipSynonymStoreIfNotExist(rightStatementRelationship);

        this->relationshipSynonymStore.at(rightStatementRelationship)->push_back(relationship);
    }
    if (Statement::isStatement(relationship->getLeftHandEntity()) && Statement::isStatement(relationship->getRightHandEntity())) {
        RelationshipSynonymKey bothStatementRelationship = RelationshipSynonymKey(&relationship->getRelationshipType(),
                                                                                  &Statement::getEntityTypeStatic(),
                                                                                  &Statement::getEntityTypeStatic());
        this->initialiseVectorForRelationshipSynonymStoreIfNotExist(bothStatementRelationship);

        this->relationshipSynonymStore.at(bothStatementRelationship)->push_back(relationship);
    }
}
