//
// Created by Aiken Wong on 9/4/23.
//

#include "RelationshipCache.h"

CacheResult::CacheResult() : first(nullptr), second(nullptr), areRelationshipsIndividuallyCached(false) {}
CacheResult::CacheResult(std::vector<Entity *> *entityVector, std::vector<Relationship *> *relationshipVector, bool areResultsIndividuallyCached) : first(entityVector), second(relationshipVector), areRelationshipsIndividuallyCached(areResultsIndividuallyCached) {}

RelationshipCache::RelationshipCache(EntityManager *entityManager) : entityManager(entityManager) {}

auto RelationshipCache::getCachedRelationship(RelationshipKey &key)
    -> Relationship * {
  auto relationshipTypeKey = static_cast<int>(key.getRelationshipType()->getKey());

  if (relationshipMapCache.size() <= relationshipTypeKey) {
    return nullptr;
  }

  int leftStatementNumber = *key.getLeftEntityKey()->getOptionalInt();

  if (relationshipMapCache[relationshipTypeKey].size() <= leftStatementNumber) {
    return nullptr;
  }

  int rightStatementNumber = *key.getRightEntityKey()->getOptionalInt();

  if (relationshipMapCache[relationshipTypeKey][leftStatementNumber].size() <= rightStatementNumber) {
    return nullptr;
  }

  auto *cachedRelationhip = relationshipMapCache[relationshipTypeKey][leftStatementNumber][rightStatementNumber];

  if (cachedRelationhip == nullptr) {
      return nullptr;
  }

  const auto *leftEntityType = key.getLeftEntityKey()->entityType;

  bool isCorrectLeftEntityType =  leftEntityType != nullptr && (*leftEntityType == Statement::getEntityTypeStatic()  || cachedRelationhip->getLeftHandEntity()->getEntityType() == *leftEntityType);

  const auto *rightEntityType = key.getRightEntityKey()->entityType;
  bool isCorrectRightEntityType = rightEntityType != nullptr && (*rightEntityType == Statement::getEntityTypeStatic() || cachedRelationhip->getRightHandEntity()->getEntityType() == *rightEntityType);

  if (!isCorrectLeftEntityType || !isCorrectRightEntityType) {
    return nullptr;
  }

  return cachedRelationhip;
}

void RelationshipCache::clearCache() {
  this->relationshipDoubleSynonymCache.clear();
  this->relationshipLiteralSynonymCache.clear();
  this->relationshipSynonymLiteralCache.clear();
  this->relationshipMapCache.clear();
  this->relationshipVectorCacheOwner.clear();
  this->entityVectorCacheOwner.clear();
  this->relationshipCacheOwner.clear();
}

auto RelationshipCache::getCachedEntitiesAndRelationships(
    std::vector<std::vector<std::vector<CacheResult>>>
        &store,
    const RelationshipType &relationshipType, const EntityType &leftHandEntityType,
    EntityKey &rightHandEntityKey)
    -> CacheResult & {
  // check if vector size is big enough
  if (store.size() <= relationshipType.getKey()) {
    store.resize(relationshipType.getKey() + 1);
  }
  if (store[relationshipType.getKey()].size() <= leftHandEntityType.getKey()) {
    store[relationshipType.getKey()].resize(leftHandEntityType.getKey() + 1);
  }

  if (store[relationshipType.getKey()][leftHandEntityType.getKey()].empty()) {
    store[relationshipType.getKey()][leftHandEntityType.getKey()].resize(
        entityManager->getNumberOfStatements() + 1);
  }

  // assign pair to cache vector
  return store[relationshipType.getKey()][leftHandEntityType.getKey()]
              [*rightHandEntityKey.getOptionalInt()];
}

void RelationshipCache::storeInRelationshipVectorOwnerCache(
    std::unique_ptr<std::vector<Relationship *>> relationships) {
  this->relationshipVectorCacheOwner.push_back(std::move(relationships));
}

void RelationshipCache::storeInEntityVectorOwnerCache(
    std::unique_ptr<std::vector<Entity *>> entities) {
  this->entityVectorCacheOwner.push_back(std::move(entities));
}

void RelationshipCache::storeInRelationshipOwnerCache(
    std::unique_ptr<Relationship> relationship) {
  this->relationshipCacheOwner.push_back(std::move(relationship));
}

auto RelationshipCache::getStoreAndCacheSizes() -> std::vector<int> {
  return {static_cast<int>(this->relationshipDoubleSynonymCache.size()),
          static_cast<int>(this->relationshipLiteralSynonymCache.size()),
          static_cast<int>(this->relationshipSynonymLiteralCache.size()),
          static_cast<int>(this->relationshipMapCache.size()),
          static_cast<int>(this->relationshipVectorCacheOwner.size()),
          static_cast<int>(this->entityVectorCacheOwner.size()),
          static_cast<int>(this->relationshipCacheOwner.size())};
}

void RelationshipCache::storeInRelationshipMapCache(
    Relationship *relationship) {
  if (relationship == nullptr) {
    return;
  }
  int relationshipTypeKey = static_cast<int>(relationship->getRelationshipType().getKey());

  if (relationshipMapCache.size() <= relationshipTypeKey) {
    relationshipMapCache.resize(relationshipTypeKey + 1);
  }
  if (relationshipMapCache[relationshipTypeKey].empty()) {
    relationshipMapCache[relationshipTypeKey].resize(
        entityManager->getNumberOfStatements() + 1);
  }
  int leftStmtNumber = *relationship->getLeftHandEntity()->getEntityKey().getOptionalInt();
  int rightStmtNumber = *relationship->getRightHandEntity()->getEntityKey().getOptionalInt();

  if (relationshipMapCache[relationshipTypeKey][leftStmtNumber].empty()) {
    relationshipMapCache[relationshipTypeKey][leftStmtNumber].resize(
        entityManager->getNumberOfStatements() + 1);
  }

  auto &cachedRelationship = relationshipMapCache[relationshipTypeKey][leftStmtNumber][rightStmtNumber];

  if (cachedRelationship == nullptr) {
    cachedRelationship = relationship;
  }
}

auto RelationshipCache::getCachedRelationshipsByTypes(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType)
    -> std::vector<Relationship *> * {
  RelationshipDoubleSynonymKey relationshipDoubleSynonymKey =
      RelationshipDoubleSynonymKey(&relationshipType, &leftHandEntityType,
                                   &rightHandEntityType);

  if (this->relationshipDoubleSynonymCache.find(relationshipDoubleSynonymKey) !=
      this->relationshipDoubleSynonymCache.end()) {
    return this->relationshipDoubleSynonymCache.at(
        relationshipDoubleSynonymKey);
  }

  return nullptr;
}

void RelationshipCache::storeInRelationshipDoubleSynonymCache(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, const EntityType &rightHandEntityType,
    std::unique_ptr<std::vector<Relationship *>> relationships) {
  if (!this->relationshipDoubleSynonymCache
           .try_emplace(RelationshipDoubleSynonymKey(&relationshipType,
                                                     &leftHandEntityType,
                                                     &rightHandEntityType),
                        relationships.get())
           .second) {
    return;
  }

  relationshipVectorCacheOwner.push_back(std::move(relationships));
}

auto RelationshipCache::getCachedResultsFromSynonymLiteralCache(
    const RelationshipType &relationshipType,
    const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
    -> CacheResult & {
  return this->getCachedEntitiesAndRelationships(
      relationshipSynonymLiteralCache, relationshipType, leftHandEntityType,
      rightHandEntityKey);
}

auto RelationshipCache::getCachedResultsFromLiteralSynonymCache(
    const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
    const EntityType &rightHandEntityType)
    -> CacheResult & {
  return this->getCachedEntitiesAndRelationships(
      relationshipLiteralSynonymCache, relationshipType, rightHandEntityType,
      leftHandEntityKey);
}
