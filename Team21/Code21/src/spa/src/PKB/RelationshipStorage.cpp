//
// Created by Aiken Wong on 26/3/23.
//

#include "RelationshipStorage.h"

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
  this->placeholderEntityVector = std::make_unique<std::vector<Entity *>>();
  this->placeholderRelationshipVector =
      std::make_unique<std::vector<Relationship *>>();
}

void RelationshipStorage::storeRelationship(
    std::unique_ptr<Relationship> relationship) {
  auto *relationshipPtr = relationship.get();

  if (!this->relationshipStore
           .try_emplace(relationship->getRelationshipKey(), relationshipPtr)
           .second) {
    return;
  }
  relationshipStoreOwner.push_back(std::move(relationship));

  this->storeInRelationshipDoubleSynonymStore(relationshipPtr);
  this->storeInRelationshipLiteralSynonymStore(relationshipPtr);
  this->storeInRelationshipSynonymLiteralStore(relationshipPtr);
}


auto RelationshipStorage::getRelationshipFromStore(RelationshipKey &key)
    -> Relationship * {
  if (this->relationshipStore.find(key) != this->relationshipStore.end()) {
    return this->relationshipStore.at(key);
  }
  return nullptr;
}

auto RelationshipStorage::getRelationshipsByTypesFromStore(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  RelationshipDoubleSynonymKey relationshipSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);

  if (this->relationshipDoubleSynonymStore.find(relationshipSynonymKey) !=
      this->relationshipDoubleSynonymStore.end()) {
    return this->relationshipDoubleSynonymStore.at(relationshipSynonymKey);
  }

  return nullptr;
}

auto RelationshipStorage::getEntitiesFromSynonymLiteralStore(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
    -> std::vector<Entity *> * {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(&relationshipType, &leftHandEntityType,
                                    &rightHandEntityKey);

  if (this->relationshipSynonymLiteralStore.find(
          relationshipSynonymLiteralKey) !=
      this->relationshipSynonymLiteralStore.end()) {
    return this->relationshipSynonymLiteralStore.at(
        relationshipSynonymLiteralKey);
  }

  return nullptr;
}

auto RelationshipStorage::getEntitiesFromLiteralSynonymStore(
    const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
    const EntityType &rightHandEntityType) -> std::vector<Entity *> * {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(&relationshipType, &leftHandEntityKey,
                                    &rightHandEntityType);

  if (this->relationshipLiteralSynonymStore.find(
          relationshipLiteralSynonymKey) !=
      this->relationshipLiteralSynonymStore.end()) {
    return this->relationshipLiteralSynonymStore.at(
        relationshipLiteralSynonymKey);
  }

  return nullptr;
}

auto RelationshipStorage::generateRelationshipVector()
    -> std::vector<Relationship *> * {
  auto owner = std::make_unique<std::vector<Relationship *>>();
  auto *ptr = owner.get();
  this->relationshipVectorStoreOwner.push_back(std::move(owner));
  return ptr;
}
auto RelationshipStorage::generateEntityVector() -> std::vector<Entity *> * {
  auto owner = std::make_unique<std::vector<Entity *>>();
  auto *ptr = owner.get();
  this->entityVectorStoreOwner.push_back(std::move(owner));
  return ptr;
}

void RelationshipStorage::storeInRelationshipDoubleSynonymStore(
    Relationship *relationship) {
  RelationshipDoubleSynonymKey relationshipSynonymKey =
      RelationshipDoubleSynonymKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityType(),
          &relationship->getRightHandEntity()->getEntityType());

  getVectorForDoubleSynonymStore(relationshipSynonymKey)
      ->push_back(relationship);

  if (Statement::isStatement(relationship->getLeftHandEntity())) {
    RelationshipDoubleSynonymKey leftStatementRelationshipKey =
        RelationshipDoubleSynonymKey(
            &relationship->getRelationshipType(),
            &Statement::getEntityTypeStatic(),
            &relationship->getRightHandEntity()->getEntityType());
    getVectorForDoubleSynonymStore(leftStatementRelationshipKey)
        ->push_back(relationship);
  }

  if (Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipDoubleSynonymKey rightStatementRelationshipKey =
        RelationshipDoubleSynonymKey(
            &relationship->getRelationshipType(),
            &relationship->getLeftHandEntity()->getEntityType(),
            &Statement::getEntityTypeStatic());
    getVectorForDoubleSynonymStore(rightStatementRelationshipKey)
        ->push_back(relationship);
  }
  if (Statement::isStatement(relationship->getLeftHandEntity()) &&
      Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipDoubleSynonymKey bothStatementRelationshipKey =
        RelationshipDoubleSynonymKey(&relationship->getRelationshipType(),
                                     &Statement::getEntityTypeStatic(),
                                     &Statement::getEntityTypeStatic());
    getVectorForDoubleSynonymStore(bothStatementRelationshipKey)
        ->push_back(relationship);
  }
}

void RelationshipStorage::storeInRelationshipLiteralSynonymStore(
    Relationship *relationship) {
  RelationshipLiteralSynonymKey relationshipLiteralSynonymKey =
      RelationshipLiteralSynonymKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityKey(),
          &relationship->getRightHandEntity()->getEntityType());

  this->getVectorForStore(relationshipLiteralSynonymKey,
                          relationshipLiteralSynonymStore)
      ->push_back(relationship->getRightHandEntity());

  if (Statement::isStatement(relationship->getRightHandEntity())) {
    RelationshipLiteralSynonymKey rightStatementRelationshipKey =
        RelationshipLiteralSynonymKey(
            &relationship->getRelationshipType(),
            &relationship->getLeftHandEntity()->getEntityKey(),
            &Statement::getEntityTypeStatic());

    this->getVectorForStore(rightStatementRelationshipKey,
                            relationshipLiteralSynonymStore)
        ->push_back(
            relationship->getRightHandEntity());
  }
}

void RelationshipStorage::storeInRelationshipSynonymLiteralStore(
    Relationship *relationship) {
  RelationshipSynonymLiteralKey relationshipSynonymLiteralKey =
      RelationshipSynonymLiteralKey(
          &relationship->getRelationshipType(),
          &relationship->getLeftHandEntity()->getEntityType(),
          &relationship->getRightHandEntity()->getEntityKey());
  this->getVectorForStore(relationshipSynonymLiteralKey,
                          relationshipSynonymLiteralStore)
      ->push_back(relationship->getLeftHandEntity());

  if (Statement::isStatement(relationship->getLeftHandEntity())) {
    RelationshipSynonymLiteralKey leftStatementRelationshipKey =
        RelationshipSynonymLiteralKey(
            &relationship->getRelationshipType(),
            &Statement::getEntityTypeStatic(),
            &relationship->getRightHandEntity()->getEntityKey());
    this->getVectorForStore(leftStatementRelationshipKey,
                            relationshipSynonymLiteralStore)
        ->push_back(relationship->getLeftHandEntity());
  }
}

template <typename T>
auto RelationshipStorage::getVectorForStore(
    T &key, std::unordered_map<T, std::vector<Entity *> *> &store)
    -> std::vector<Entity *> * {
  auto *placeholderEntityVectorPtr = placeholderEntityVector.get();

  auto emplaceResult = store.try_emplace(key, placeholderEntityVectorPtr);

  if (emplaceResult.second) {
    entityVectorStoreOwner.push_back(std::move(placeholderEntityVector));

    placeholderEntityVector = std::make_unique<std::vector<Entity *>>();

    return placeholderEntityVectorPtr;
  }

  return emplaceResult.first->second;
}

auto RelationshipStorage::getVectorForDoubleSynonymStore(
    RelationshipDoubleSynonymKey relationshipDoubleSynonymKey)
    -> std::vector<Relationship *> * {
  auto *placeholderRelationshipVectorPtr = placeholderRelationshipVector.get();

  auto emplaceResult = this->relationshipDoubleSynonymStore.try_emplace(
      relationshipDoubleSynonymKey, placeholderRelationshipVectorPtr);

  if (emplaceResult.second) {
    relationshipVectorStoreOwner.push_back(
        std::move(placeholderRelationshipVector));

    placeholderRelationshipVector =
        std::make_unique<std::vector<Relationship *>>();

    return placeholderRelationshipVectorPtr;
  }

  return emplaceResult.first->second;
}
