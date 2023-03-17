//
// Created by Aiken Wong on 4/2/23.
//

#include "RelationshipManager.h"

#include <iostream>
#include <memory>
#include <stdexcept>

RelationshipDoubleSynonymKey::RelationshipDoubleSynonymKey(
    RelationshipType *relationshipType, const EntityType *leftHandEntityType,
    const EntityType *rightHandEntityType)
    : StorageKey(relationshipType->getKey() ^ leftHandEntityType->getKey() ^
                 rightHandEntityType->getKey()),
      relationshipType(relationshipType),
      leftHandEntityType(leftHandEntityType),
      rightHandEntityType(rightHandEntityType) {}

auto RelationshipDoubleSynonymKey::operator==(
    const RelationshipDoubleSynonymKey &otherRelationshipSynonymKey) const
    -> bool {
  return *this->relationshipType ==
             *otherRelationshipSynonymKey.relationshipType &&
         *this->leftHandEntityType ==
             *otherRelationshipSynonymKey.leftHandEntityType &&
         *this->rightHandEntityType ==
             *otherRelationshipSynonymKey.rightHandEntityType;
}

auto std::hash<RelationshipDoubleSynonymKey>::operator()(
    const RelationshipDoubleSynonymKey &RelationshipSynonymKey) const
    -> size_t {
  return RelationshipSynonymKey.getKey();
}

RelationshipLiteralSynonymKey::RelationshipLiteralSynonymKey(
    RelationshipType *relationshipType, EntityKey *entityKey,
    const EntityType *entityType)
    : StorageKey(relationshipType->getKey() ^ entityKey->getKey() ^
                 entityType->getKey()),
      relationshipType(relationshipType),
      leftHandEntityKey(entityKey),
      rightHandEntityType(entityType) {}

auto RelationshipLiteralSynonymKey::operator==(
    const RelationshipLiteralSynonymKey &otherRelationshipLiteralSynonymKey)
    const -> bool {
  return *this->relationshipType ==
             *otherRelationshipLiteralSynonymKey.relationshipType &&
         *this->leftHandEntityKey ==
             *otherRelationshipLiteralSynonymKey.leftHandEntityKey &&
         *this->rightHandEntityType ==
             *otherRelationshipLiteralSynonymKey.rightHandEntityType;
}

auto std::hash<RelationshipLiteralSynonymKey>::operator()(
    const RelationshipLiteralSynonymKey &relationshipLiteralSynonymKey) const
    -> size_t {
  return relationshipLiteralSynonymKey.getKey();
}

RelationshipSynonymLiteralKey::RelationshipSynonymLiteralKey(
    RelationshipType *relationshipType, const EntityType *entityType,
    EntityKey *entityKey)
    : StorageKey(relationshipType->getKey() ^ entityType->getKey() ^
                 entityKey->getKey()),
      relationshipType(relationshipType),
      leftHandEntityType(entityType),
      rightHandEntityKey(entityKey) {}

auto RelationshipSynonymLiteralKey::operator==(
    const RelationshipSynonymLiteralKey &otherRelationshipLiteralSynonymKey)
    const -> bool {
  return *this->relationshipType ==
             *otherRelationshipLiteralSynonymKey.relationshipType &&
         *this->leftHandEntityType ==
             *otherRelationshipLiteralSynonymKey.leftHandEntityType &&
         *this->rightHandEntityKey ==
             *otherRelationshipLiteralSynonymKey.rightHandEntityKey;
}

auto std::hash<RelationshipSynonymLiteralKey>::operator()(
    const RelationshipSynonymLiteralKey &relationshipLiteralSynonymKey) const
    -> size_t {
  return relationshipLiteralSynonymKey.getKey();
}

RelationshipManager::RelationshipManager() {
    this->relationshipDoubleSynonymStore = std::unordered_map<RelationshipDoubleSynonymKey, std::shared_ptr<std::vector<Relationship *>>>();
    this->relationshipStore = std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>>();
}

void RelationshipManager::storeRelationship(std::shared_ptr<Relationship> relationship) {
    if (relationshipStore.find(relationship->getRelationshipKey()) != relationshipStore.end()) {
        return;
    }

    this->relationshipStore.insert(std::make_pair(relationship->getRelationshipKey(), std::shared_ptr<Relationship>(relationship)));

    this->storeInRelationshipDoubleSynonymStore(relationship.get());
    this->storeInRelationshipLiteralSynonymStore(relationship.get());
    this->storeInRelationshipSynonymLiteralStore(relationship.get());
}

auto RelationshipManager::getRelationship(RelationshipKey &key)
    -> Relationship * {
  if (relationshipStore.find(key) != relationshipStore.end()) {
    return this->relationshipStore.at(key).get();
  }
  return nullptr;
}

auto RelationshipManager::getRelationshipsByTypes(
    RelationshipType &relationshipType, const EntityType &leftHandEntityType,
    const EntityType &rightHandEntityType) -> std::vector<Relationship *> * {
  RelationshipDoubleSynonymKey relationshipSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);
  this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
      relationshipSynonymKey);

  return this->relationshipDoubleSynonymStore.at(relationshipSynonymKey).get();
}

auto RelationshipManager::
    getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
        RelationshipType &relationshipType, const EntityType &leftHandEntityType,
        EntityKey &rightHandEntityKey) -> std::vector<Entity *> * {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);
  this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
      relationshipSynonymLiteralKey);

  return this->relationshipSynonymLiteralStore.at(relationshipSynonymLiteralKey)
      .get();
}

auto RelationshipManager::
    getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
        RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);
  this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
      relationshipLiteralSynonymKey);

  return this->relationshipLiteralSynonymStore.at(relationshipLiteralSynonymKey)
      .get();
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
