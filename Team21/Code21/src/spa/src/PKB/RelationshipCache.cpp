//
// Created by Aiken Wong on 9/4/23.
//

#include "RelationshipCache.h"

RelationshipCache::RelationshipCache(EntityManager *entityManager) : entityManager(entityManager) {}

auto RelationshipCache::getCachedRelationship(RelationshipKey &key)
    -> Relationship * {
  return this->relationshipCache.at(key);
}

auto RelationshipCache::containedInRelationshipCache(RelationshipKey &key)
    -> bool {
  return this->relationshipCache.find(key) != this->relationshipCache.end();
}

void RelationshipCache::clearCache() {
  this->relationshipDoubleSynonymCache.clear();
  this->relationshipLiteralSynonymCache.clear();
  this->relationshipSynonymLiteralCache.clear();
  this->relationshipCache.clear();
  this->relationshipVectorCacheOwner.clear();
  this->entityVectorCacheOwner.clear();
  this->relationshipCacheOwner.clear();
}

auto RelationshipCache::getCachedEntitiesAndRelationships(
    std::vector<std::vector<std::vector<
        std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>>>
        &store,
    const RelationshipType &relationshipType, const EntityType &givenEntityType,
    EntityKey &givenEntityKey)
    -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  // check if vector size is big enough
  if (store.size() <= relationshipType.getKey()) {
    store.resize(relationshipType.getKey() + 1);
  }
  if (store[relationshipType.getKey()].size() <= givenEntityType.getKey()) {
    store[relationshipType.getKey()].resize(givenEntityType.getKey() + 1);
  }

  if (store[relationshipType.getKey()][givenEntityType.getKey()].empty()) {
    store[relationshipType.getKey()][givenEntityType.getKey()].resize(
        entityManager->getNumberOfStatements() + 1);
  }

  // assign pair to cache vector
  return store[relationshipType.getKey()][givenEntityType.getKey()]
              [*givenEntityKey.getOptionalInt()];
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
          static_cast<int>(this->relationshipCache.size()),
          static_cast<int>(this->relationshipVectorCacheOwner.size()),
          static_cast<int>(this->entityVectorCacheOwner.size()),
          static_cast<int>(this->relationshipCacheOwner.size())};
}

void RelationshipCache::storeInRelationshipMapCache(
    Relationship *relationship) {
  if (relationship == nullptr) {
    return;
  }
  this->relationshipCache.try_emplace(relationship->getRelationshipKey(),
                                      relationship);
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
    -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  return this->getCachedEntitiesAndRelationships(
      relationshipSynonymLiteralCache, relationshipType, leftHandEntityType,
      rightHandEntityKey);
}

auto RelationshipCache::getCachedResultsFromLiteralSynonymCache(
    const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
    const EntityType &rightHandEntityType)
    -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> & {
  return this->getCachedEntitiesAndRelationships(
      relationshipLiteralSynonymCache, relationshipType, rightHandEntityType,
      leftHandEntityKey);
}
