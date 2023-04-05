//
// Created by Aiken Wong on 26/3/23.
//

#include "RelationshipStorage.h"

#include <memory>

RelationshipDoubleSynonymKey::RelationshipDoubleSynonymKey(
    const RelationshipType *relationshipType,
    const EntityType *leftHandEntityType, const EntityType *rightHandEntityType)
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
    const RelationshipType *relationshipType, EntityKey *entityKey,
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
    const RelationshipType *relationshipType, const EntityType *entityType,
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

RelationshipStorage::RelationshipStorage() {
  this->relationshipDoubleSynonymStore =
      std::unordered_map<RelationshipDoubleSynonymKey,
                         std::shared_ptr<std::vector<Relationship *>>>();
  this->relationshipStore =
      std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>>();
}

void RelationshipStorage::storeRelationship(
    const std::shared_ptr<Relationship> &relationship) {
  if (!this->relationshipStore
           .try_emplace(relationship->getRelationshipKey(),
                        std::shared_ptr<Relationship>(relationship))
           .second) {
    // early return if relationship already exists
    return;
  }
  this->tryStoreRelationshipOnlyInRelationshipStore(relationship, false);
  this->storeInRelationshipDoubleSynonymStore(relationship.get());
  this->storeInRelationshipLiteralSynonymStore(relationship.get(), false);
  this->storeInRelationshipSynonymLiteralStore(relationship.get(), false);
}

auto RelationshipStorage::tryStoreRelationshipOnlyInRelationshipStore(
    const std::shared_ptr<Relationship> &relationship, bool useCache) -> bool {
  if (useCache) {
    // return this->relationshipCache
    //     .try_emplace(relationship->getRelationshipKey(), relationship)
    //     .second;
  }
  return this->relationshipStore
      .try_emplace(relationship->getRelationshipKey(), relationship)
      .second;
}

auto RelationshipStorage::getCachedRelationship(RelationshipKey &key)
    -> Relationship * {
  return this->relationshipCache.at(key);
}

auto RelationshipStorage::containedInRelationshipCache(RelationshipKey &key)
    -> bool {
  return this->relationshipCache.find(key) != this->relationshipCache.end();
}

auto RelationshipStorage::getRelationship(RelationshipKey &key)
    -> Relationship * {
  if (this->relationshipStore.find(key) != this->relationshipStore.end()) {
    return this->relationshipStore.at(key).get();
  }
  // if (this->relationshipCache.find(key) != this->relationshipCache.end()) {
  //   return this->relationshipCache.at(key);
  // }
  return nullptr;
}

auto RelationshipStorage::getRelationshipsByTypes(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  RelationshipDoubleSynonymKey relationshipSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);

  if (this->relationshipDoubleSynonymStore.find(relationshipSynonymKey) !=
      this->relationshipDoubleSynonymStore.end()) {
    return this->relationshipDoubleSynonymStore.at(relationshipSynonymKey)
        .get();
  }
  if (this->relationshipDoubleSynonymCache.find(relationshipSynonymKey) !=
      this->relationshipDoubleSynonymCache.end()) {
    return this->relationshipDoubleSynonymCache.at(relationshipSynonymKey)
        .get();
  }
  return nullptr;
}

auto RelationshipStorage::
    getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
        const RelationshipType &relationshipType,
        const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
        -> std::vector<Entity *> * {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);

  if (this->relationshipSynonymLiteralStore.find(
          relationshipSynonymLiteralKey) !=
      this->relationshipSynonymLiteralStore.end()) {
    return this->relationshipSynonymLiteralStore
        .at(relationshipSynonymLiteralKey)
        .get();
  }
  if (this->relationshipSynonymLiteralCache.find(
          relationshipSynonymLiteralKey) !=
      this->relationshipSynonymLiteralCache.end()) {
    return this->relationshipSynonymLiteralCache
        .at(relationshipSynonymLiteralKey)
        .get();
  }
  return nullptr;
}

auto RelationshipStorage::
    getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
        const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  if (this->relationshipLiteralSynonymStore.find(
          relationshipLiteralSynonymKey) !=
      this->relationshipLiteralSynonymStore.end()) {
    return this->relationshipLiteralSynonymStore
        .at(relationshipLiteralSynonymKey)
        .get();
  }
  if (this->relationshipLiteralSynonymCache.find(
          relationshipLiteralSynonymKey) !=
      this->relationshipLiteralSynonymCache.end()) {
    return this->relationshipLiteralSynonymCache
        .at(relationshipLiteralSynonymKey)
        .get();
  }
  return nullptr;
}

void RelationshipStorage::
    initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
        const RelationshipDoubleSynonymKey &relationshipSynonymKey,
        bool useCache) {
  // inserts only if key doesn't exist

  if (useCache) {
    // this->relationshipDoubleSynonymCache.try_emplace(
    //     relationshipSynonymKey,
    //     std::make_shared<std::vector<Relationship *>>());
  } else {
    this->relationshipDoubleSynonymStore.try_emplace(
        relationshipSynonymKey,
        std::make_shared<std::vector<Relationship *>>());
  }
}

void RelationshipStorage::
    initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
        RelationshipLiteralSynonymKey relationshipLiteralSynonymKey,
        bool useCache) {
  // inserts only if key doesn't exist

  if (useCache) {
    // this->relationshipLiteralSynonymCache.try_emplace(
    //     relationshipLiteralSynonymKey,
    //     std::make_shared<std::vector<Entity *>>());
  } else {
    this->relationshipLiteralSynonymStore.try_emplace(
        relationshipLiteralSynonymKey,
        std::make_shared<std::vector<Entity *>>());
  }
}

void RelationshipStorage::
    initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
        const RelationshipSynonymLiteralKey &relationshipSynonymLiteralKey,
        bool useCache) {
  // inserts only if key doesn't exist
  if (useCache) {
    // this->relationshipSynonymLiteralCache.try_emplace(
    //     relationshipSynonymLiteralKey,
    //     std::make_shared<std::vector<Entity *>>());
  } else {
    this->relationshipSynonymLiteralStore.try_emplace(
        relationshipSynonymLiteralKey,
        std::make_shared<std::vector<Entity *>>());
  }
}

void RelationshipStorage::storeInSpecifiedRelationshipDoubleSynonymStore(
    Relationship *relationship, const RelationshipDoubleSynonymKey &key,
    bool useCache) {
  this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(key,
                                                                    useCache);
  if (useCache) {
    // this->relationshipDoubleSynonymCache.at(key)->push_back(relationship);
  } else {
    this->relationshipDoubleSynonymStore.at(key)->push_back(relationship);
  }
}

void RelationshipStorage::storeInRelationshipDoubleSynonymStore(
    Relationship *relationship) {
  RelationshipDoubleSynonymKey relationshipSynonymKey =
      RelationshipDoubleSynonymKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityType(),
          &relationship->getRightHandEntity()->getEntityType());

  this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
      relationshipSynonymKey, false);

  this->relationshipDoubleSynonymStore.at(relationshipSynonymKey)
      ->push_back(relationship);

  if (Statement::isStatement(relationship->getLeftHandEntity())) {
    RelationshipDoubleSynonymKey leftStatementRelationship =
        RelationshipDoubleSynonymKey(
            &relationship->getRelationshipType(),
            &Statement::getEntityTypeStatic(),
            &relationship->getRightHandEntity()->getEntityType());
    this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
        leftStatementRelationship, false);

    this->relationshipDoubleSynonymStore.at(leftStatementRelationship)
        ->push_back(relationship);
  }
  if (Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipDoubleSynonymKey rightStatementRelationship =
        RelationshipDoubleSynonymKey(
            &relationship->getRelationshipType(),
            &relationship->getLeftHandEntity()->getEntityType(),
            &Statement::getEntityTypeStatic());
    this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
        rightStatementRelationship, false);

    this->relationshipDoubleSynonymStore.at(rightStatementRelationship)
        ->push_back(relationship);
  }
  if (Statement::isStatement(relationship->getLeftHandEntity()) &&
      Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipDoubleSynonymKey bothStatementRelationship =
        RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                     &Statement::getEntityTypeStatic(),
                                     &Statement::getEntityTypeStatic());
    this->initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
        bothStatementRelationship, false);

    this->relationshipDoubleSynonymStore.at(bothStatementRelationship)
        ->push_back(relationship);
  }
}

void RelationshipStorage::storeInRelationshipLiteralSynonymStore(
    Relationship *relationship, bool useCache) {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityKey(),
          &relationship->getRightHandEntity()->getEntityType());

  this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
      relationshipLiteralSynonymKey, useCache);

  if (useCache) {
    // this->relationshipLiteralSynonymCache.at(relationshipLiteralSynonymKey)
    //     ->push_back(relationship->getRightHandEntity());
  } else {
    this->relationshipLiteralSynonymStore.at(relationshipLiteralSynonymKey)
        ->push_back(relationship->getRightHandEntity());
  }
  if (Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipLiteralSynonymKey rightStatementRelationship =
        RelationshipLiteralSynonymKey(
            &relationship->getRelationshipType(),
            &relationship->getLeftHandEntity()->getEntityKey(),
            &Statement::getEntityTypeStatic());

    this->initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
        rightStatementRelationship, useCache);
    if (useCache) {
      // this->relationshipLiteralSynonymCache.at(rightStatementRelationship)
      //     ->push_back(relationship->getRightHandEntity());
    } else {
      this->relationshipLiteralSynonymStore.at(rightStatementRelationship)
          ->push_back(relationship->getRightHandEntity());
    }
  }
}

void RelationshipStorage::storeInRelationshipSynonymLiteralStore(
    Relationship *relationship, bool useCache) {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityType(),
          &relationship->getRightHandEntity()->getEntityKey());

  this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
      relationshipSynonymLiteralKey, useCache);

  if (useCache) {
    // this->relationshipSynonymLiteralCache.at(relationshipSynonymLiteralKey)
    //     ->push_back(relationship->getLeftHandEntity());
  } else {
    this->relationshipSynonymLiteralStore.at(relationshipSynonymLiteralKey)
        ->push_back(relationship->getLeftHandEntity());
  }

  if (Statement::isStatement(relationship->getLeftHandEntity())) {
    RelationshipSynonymLiteralKey leftStatementRelationship =
        RelationshipSynonymLiteralKey(
            &relationship->getRelationshipType(),
            &Statement::getEntityTypeStatic(),
            &relationship->getRightHandEntity()->getEntityKey());
    this->initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
        leftStatementRelationship, useCache);

    if (useCache) {
      // this->relationshipSynonymLiteralCache.at(leftStatementRelationship)
      //     ->push_back(relationship->getLeftHandEntity());
    } else {
      this->relationshipSynonymLiteralStore.at(leftStatementRelationship)
          ->push_back(relationship->getLeftHandEntity());
    }
  }
}

void RelationshipStorage::clearCache() {
  this->relationshipDoubleSynonymCache.clear();
  this->relationshipLiteralSynonymCache.clear();
  this->relationshipSynonymLiteralCache.clear();
  this->relationshipVectorLiteralSynonymCache.clear();
  this->relationshipVectorSynonymLiteralCache.clear();
  this->relationshipCache.clear();
}

auto RelationshipStorage::getStoreAndCacheSizes() -> std::vector<int> {
  return std::vector<int>{
      static_cast<int>(this->relationshipDoubleSynonymStore.size()),
      static_cast<int>(this->relationshipLiteralSynonymStore.size()),
      static_cast<int>(this->relationshipSynonymLiteralStore.size()),
      static_cast<int>(this->relationshipStore.size()),
      static_cast<int>(this->relationshipDoubleSynonymCache.size()),
      static_cast<int>(this->relationshipLiteralSynonymCache.size()),
      static_cast<int>(this->relationshipSynonymLiteralCache.size()),
      static_cast<int>(this->relationshipCache.size()),
      static_cast<int>(this->relationshipVectorLiteralSynonymCache.size()),
      static_cast<int>(this->relationshipVectorSynonymLiteralCache.size())};
}

void RelationshipStorage::storeInRelationshipLiteralSynonymCache(
    const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
    const EntityType &rightHandEntityType,
    std::shared_ptr<std::vector<Entity *>> relatedEntities) {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  this->relationshipLiteralSynonymCache.try_emplace(
      relationshipLiteralSynonymKey, relatedEntities);
}

void RelationshipStorage::storeInRelationshipSynonymLiteralCache(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey,
    std::shared_ptr<std::vector<Entity *>> relatedEntities) {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);
  this->relationshipSynonymLiteralCache.try_emplace(
      relationshipSynonymLiteralKey, relatedEntities);
}

void RelationshipStorage::storeInRelationshipLiteralSynonymCache(
    const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
    const EntityType &rightHandEntityType,
    std::shared_ptr<std::vector<Relationship *>> relationships) {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  this->relationshipVectorLiteralSynonymCache.try_emplace(
      relationshipLiteralSynonymKey, relationships);
}

void RelationshipStorage::storeInRelationshipSynonymLiteralCache(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey,
    std::shared_ptr<std::vector<Relationship *>> relationships) {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);
  this->relationshipVectorSynonymLiteralCache.try_emplace(
      relationshipSynonymLiteralKey, relationships);
}

void RelationshipStorage::storeInRelationshipDoubleSynonymCache(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType,
    std::shared_ptr<std::vector<Relationship *>> relationships) {
  RelationshipDoubleSynonymKey relationshipDoubleSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);
  this->relationshipDoubleSynonymCache.try_emplace(relationshipDoubleSynonymKey,
                                                   relationships);
}

auto RelationshipStorage::
    getCachedEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
        const RelationshipType &relationshipType,
        const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
        -> std::vector<Entity *> * {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);

  if (this->relationshipSynonymLiteralCache.find(
          relationshipSynonymLiteralKey) !=
      this->relationshipSynonymLiteralCache.end()) {
    return this->relationshipSynonymLiteralCache
        .at(relationshipSynonymLiteralKey)
        .get();
  }

  return nullptr;
}

auto RelationshipStorage::
    getCachedEntitiesForGivenRelationshipTypeAndRightHandEntityType(
        const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  if (this->relationshipLiteralSynonymCache.find(
          relationshipLiteralSynonymKey) !=
      this->relationshipLiteralSynonymCache.end()) {
    return this->relationshipLiteralSynonymCache
        .at(relationshipLiteralSynonymKey)
        .get();
  }

    return nullptr;

}

auto RelationshipStorage::getCachedRelationshipsByTypes(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  RelationshipDoubleSynonymKey relationshipDoubleSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);

  if (this->relationshipDoubleSynonymCache.find(relationshipDoubleSynonymKey) !=
      this->relationshipDoubleSynonymCache.end()) {
    return this->relationshipDoubleSynonymCache.at(relationshipDoubleSynonymKey)
        .get();
  }

    return nullptr;

}

auto RelationshipStorage::
    getCachedRelationshipsForGivenRelationshipTypeAndRightHandEntityType(
        const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
        const EntityType &rightHandEntityType)
        -> std::vector<Relationship *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  if (this->relationshipVectorLiteralSynonymCache.find(
          relationshipLiteralSynonymKey) !=
      this->relationshipVectorLiteralSynonymCache.end()) {
    return this->relationshipVectorLiteralSynonymCache
        .at(relationshipLiteralSynonymKey)
        .get();
  }

    return nullptr;

}

auto RelationshipStorage::
    getCachedRelationshipsForGivenRelationshipTypeAndLeftHandEntityType(
        const RelationshipType &relationshipType,
        const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
        -> std::vector<Relationship *> * {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);

  if (this->relationshipVectorSynonymLiteralCache.find(
          relationshipSynonymLiteralKey) !=
      this->relationshipVectorSynonymLiteralCache.end()) {
    return this->relationshipVectorSynonymLiteralCache
        .at(relationshipSynonymLiteralKey)
        .get();
  }

    return nullptr;

}

void RelationshipStorage::storeInRelationshipOwnerCache(
    std::shared_ptr<Relationship> relationship) {
  this->relationshipOwnerCache.push_back(relationship);
}

void RelationshipStorage::storeInRelationshipMapCache(
    Relationship *relationship) {
  this->relationshipCache.try_emplace(relationship->getRelationshipKey(),
                                      relationship);
}
