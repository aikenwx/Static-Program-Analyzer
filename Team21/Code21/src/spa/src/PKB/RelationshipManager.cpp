//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <iostream>
#include <memory>
#include <stdexcept>

RelationshipDoubleSynonymKey::RelationshipDoubleSynonymKey(RelationshipType *relationshipType, EntityType *leftHandEntityType, EntityType *rightHandEntityType) : StorageKey(relationshipType->getKey() ^ leftHandEntityType->getKey() ^ rightHandEntityType->getKey()) {
    this->relationshipType = relationshipType;
    this->leftHandEntityType = leftHandEntityType;
    this->rightHandEntityType = rightHandEntityType;
}

bool RelationshipDoubleSynonymKey::operator==(const RelationshipDoubleSynonymKey &otherRelationshipSynonymKey) const {
    return *this->relationshipType == *otherRelationshipSynonymKey.relationshipType &&
           *this->leftHandEntityType == *otherRelationshipSynonymKey.leftHandEntityType &&
           *this->rightHandEntityType == *otherRelationshipSynonymKey.rightHandEntityType;
}

size_t std::hash<RelationshipDoubleSynonymKey>::operator()(const RelationshipDoubleSynonymKey &RelationshipSynonymKey) const {
    return RelationshipSynonymKey.getKey();
}

RelationshipLiteralSynonymKey::RelationshipLiteralSynonymKey(RelationshipType *relationshipType, EntityKey *entityKey, EntityType *entityType) : StorageKey(relationshipType->getKey() ^ entityKey->getKey() ^ entityType->getKey()) {
    this->relationshipType = relationshipType;
    this->leftHandEntityKey = entityKey;
    this->rightHandEntityType = entityType;
}

bool RelationshipLiteralSynonymKey::operator==(const RelationshipLiteralSynonymKey &otherRelationshipLiteralSynonymKey) const {
    return *this->relationshipType == *otherRelationshipLiteralSynonymKey.relationshipType &&
           *this->leftHandEntityKey == *otherRelationshipLiteralSynonymKey.leftHandEntityKey &&
           *this->rightHandEntityType == *otherRelationshipLiteralSynonymKey.rightHandEntityType;
}

size_t std::hash<RelationshipLiteralSynonymKey>::operator()(const RelationshipLiteralSynonymKey &relationshipLiteralSynonymKey) const {
    return relationshipLiteralSynonymKey.getKey();
}

RelationshipSynonymLiteralKey::RelationshipSynonymLiteralKey(RelationshipType *relationshipType, EntityType *entityType, EntityKey *entityKey) : StorageKey(relationshipType->getKey() ^ entityType->getKey() ^ entityKey->getKey()) {
    this->relationshipType = relationshipType;
    this->leftHandEntityType = entityType;
    this->rightHandEntityKey = entityKey;
}

bool RelationshipSynonymLiteralKey::operator==(const RelationshipSynonymLiteralKey &otherRelationshipLiteralSynonymKey) const {
    return *this->relationshipType == *otherRelationshipLiteralSynonymKey.relationshipType &&
           *this->leftHandEntityType == *otherRelationshipLiteralSynonymKey.leftHandEntityType &&
           *this->rightHandEntityKey == *otherRelationshipLiteralSynonymKey.rightHandEntityKey;
}

size_t std::hash<RelationshipSynonymLiteralKey>::operator()(const RelationshipSynonymLiteralKey &relationshipLiteralSynonymKey) const {
    return relationshipLiteralSynonymKey.getKey();
}

RelationshipManager::RelationshipManager() {
    this->relationshipDoubleSynonymStore = std::unordered_map<RelationshipDoubleSynonymKey, std::shared_ptr<std::vector<Relationship *>>>();
    this->relationshipStore = std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>>();
}

void RelationshipManager::storeRelationship(Relationship *relationship) {
    if (relationshipStore.find(relationship->getRelationshipKey()) != relationshipStore.end()) {
        delete relationship;
        return;
    }

    this->relationshipStore.insert(std::make_pair(relationship->getRelationshipKey(), std::shared_ptr<Relationship>(relationship)));

    this->storeInRelationshipDoubleSynonymStore(relationship);
    this->storeInRelationshipLiteralSynonymStore(relationship);
    this->storeInRelationshipSynonymLiteralStore(relationship);
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
    RelationshipDoubleSynonymKey relationshipSynonymKey = RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType, &rightHandEntityType);
    this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(relationshipSynonymKey);

    return this->relationshipDoubleSynonymStore.at(relationshipSynonymKey).get();
}

std::vector<Entity *> *
RelationshipManager::getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(RelationshipType &relationshipType, EntityType &leftHandEntityType,
                                                                              EntityKey &rightHandEntityKey) {
    RelationshipSynonymLiteralKey relationshipSynonymLiteralKey = RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType, &rightHandEntityKey);
    this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(relationshipSynonymLiteralKey);

    return this->relationshipSynonymLiteralStore.at(relationshipSynonymLiteralKey).get();
}

std::vector<Entity *> *
RelationshipManager::getEntitiesForGivenRelationshipTypeAndRightHandEntityType(RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
                                                                               EntityType &rightHandEntityType) {
    RelationshipLiteralSynonymKey relationshipLiteralSynonymKey = RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey, &rightHandEntityType);
    this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(relationshipLiteralSynonymKey);

    return this->relationshipLiteralSynonymStore.at(relationshipLiteralSynonymKey).get();
}

void RelationshipManager::initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
    RelationshipDoubleSynonymKey relationshipSynonymKey) {
    if (this->relationshipDoubleSynonymStore.find(relationshipSynonymKey) == this->relationshipDoubleSynonymStore.end()) {
        this->relationshipDoubleSynonymStore.insert({relationshipSynonymKey, std::make_shared<std::vector<Relationship *>>()});
    }
}

void RelationshipManager::initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
    RelationshipLiteralSynonymKey relationshipLiteralSynonymKey) {
    if (this->relationshipLiteralSynonymStore.find(relationshipLiteralSynonymKey) == this->relationshipLiteralSynonymStore.end()) {
        this->relationshipLiteralSynonymStore.insert({relationshipLiteralSynonymKey, std::make_shared<std::vector<Entity *>>()});
    }
}

void RelationshipManager::initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
    RelationshipSynonymLiteralKey relationshipSynonymLiteralKey) {
    if (this->relationshipSynonymLiteralStore.find(relationshipSynonymLiteralKey) == this->relationshipSynonymLiteralStore.end()) {
        this->relationshipSynonymLiteralStore.insert({relationshipSynonymLiteralKey, std::make_shared<std::vector<Entity *>>()});
    }
}

void RelationshipManager::storeInRelationshipDoubleSynonymStore(Relationship *relationship) {
    RelationshipDoubleSynonymKey relationshipSynonymKey = RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                                                                       &relationship->getLeftHandEntity()->getEntityType(),
                                                                                       &relationship->getRightHandEntity()->getEntityType());

    this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(relationshipSynonymKey);

    this->relationshipDoubleSynonymStore.at(relationshipSynonymKey)->push_back(relationship);

    if (Statement::isStatement(relationship->getLeftHandEntity())) {
        RelationshipDoubleSynonymKey leftStatementRelationship = RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                                                                              &Statement::getEntityTypeStatic(),
                                                                                              &relationship->getRightHandEntity()->getEntityType());
        this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(leftStatementRelationship);

        this->relationshipDoubleSynonymStore.at(leftStatementRelationship)->push_back(relationship);
    }
    if (Statement::isStatement(relationship->getRightHandEntity())) {
        RelationshipDoubleSynonymKey rightStatementRelationship = RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                                                                               &relationship->getLeftHandEntity()->getEntityType(),
                                                                                               &Statement::getEntityTypeStatic());
        this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(rightStatementRelationship);

        this->relationshipDoubleSynonymStore.at(rightStatementRelationship)->push_back(relationship);
    }
    if (Statement::isStatement(relationship->getLeftHandEntity()) && Statement::isStatement(relationship->getRightHandEntity())) {
        RelationshipDoubleSynonymKey bothStatementRelationship = RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                                                                              &Statement::getEntityTypeStatic(),
                                                                                              &Statement::getEntityTypeStatic());
        this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(bothStatementRelationship);

        this->relationshipDoubleSynonymStore.at(bothStatementRelationship)->push_back(relationship);
    }
}

void RelationshipManager::storeInRelationshipLiteralSynonymStore(Relationship *relationship) {
    RelationshipLiteralSynonymKey relationshipLiteralSynonymKey = RelationshipLiteralSynonymKey(&relationship->getRelationshipType(),
                                                                                                &relationship->getLeftHandEntity()->getEntityKey(),
                                                                                                &relationship->getRightHandEntity()->getEntityType());

    this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(relationshipLiteralSynonymKey);

    this->relationshipLiteralSynonymStore.at(relationshipLiteralSynonymKey)->push_back(relationship->getRightHandEntity());

    if (Statement::isStatement(relationship->getRightHandEntity())) {
        RelationshipLiteralSynonymKey rightStatementRelationship = RelationshipLiteralSynonymKey(&relationship->getRelationshipType(),
                                                                                                 &relationship->getLeftHandEntity()->getEntityKey(),
                                                                                                 &Statement::getEntityTypeStatic());
        this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(rightStatementRelationship);

        this->relationshipLiteralSynonymStore.at(rightStatementRelationship)->push_back(relationship->getRightHandEntity());
    }
}

void RelationshipManager::storeInRelationshipSynonymLiteralStore(Relationship *relationship) {
    RelationshipSynonymLiteralKey relationshipSynonymLiteralKey = RelationshipSynonymLiteralKey(&relationship->getRelationshipType(),
                                                                                                &relationship->getLeftHandEntity()->getEntityType(),
                                                                                                &relationship->getRightHandEntity()->getEntityKey());

    this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(relationshipSynonymLiteralKey);

    this->relationshipSynonymLiteralStore.at(relationshipSynonymLiteralKey)->push_back(relationship->getLeftHandEntity());

    if (Statement::isStatement(relationship->getLeftHandEntity())) {
        RelationshipSynonymLiteralKey leftStatementRelationship = RelationshipSynonymLiteralKey(&relationship->getRelationshipType(),
                                                                                                &Statement::getEntityTypeStatic(),
                                                                                                &relationship->getRightHandEntity()->getEntityKey());
        this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(leftStatementRelationship);

        this->relationshipSynonymLiteralStore.at(leftStatementRelationship)->push_back(relationship->getLeftHandEntity());
    }
}
