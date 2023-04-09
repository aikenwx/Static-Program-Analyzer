//
// Created by Aiken Wong on 9/4/23.
//

#ifndef SPA_RELATIONSHIPCACHE_H
#define SPA_RELATIONSHIPCACHE_H

#include "RelationshipStorage.h"

class RelationshipCache {
 private:
  EntityManager *entityManager;

  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::vector<Relationship *> *>
      relationshipDoubleSynonymCache;

  std::unordered_map<RelationshipKey, Relationship *> relationshipCache;

  std::vector<std::unique_ptr<Relationship>> relationshipCacheOwner;

  std::vector<std::unique_ptr<std::vector<Relationship *>>>
      relationshipVectorCacheOwner;

  std::vector<std::unique_ptr<std::vector<Entity *>>> entityVectorCacheOwner;

  // relationshipType -> stmttype -> stmt -> vector
  std::vector<std::vector<std::vector<
      std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>>>
      relationshipLiteralSynonymCache;

  // relationshipType -> stmttype -> stmt -> vector
  std::vector<std::vector<std::vector<
      std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>>>
      relationshipSynonymLiteralCache;

 public:
  RelationshipCache(EntityManager *entityManager);

  void storeInRelationshipOwnerCache(
      std::unique_ptr<Relationship> relationship);

  void storeInRelationshipVectorOwnerCache(
      std::unique_ptr<std::vector<Relationship *>> relationships);

  void storeInEntityVectorOwnerCache(
      std::unique_ptr<std::vector<Entity *>> entities);

  void storeInRelationshipMapCache(Relationship *relationship);

  void storeInRelationshipDoubleSynonymCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType,
      const EntityType &rightHandEntityType,
      std::unique_ptr<std::vector<Relationship *>> relationships);

  auto getCachedRelationship(RelationshipKey &key) -> Relationship *;

  auto containedInRelationshipCache(RelationshipKey &key) -> bool;

  auto getCachedResultsFromSynonymLiteralCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> &;

  auto getCachedResultsFromLiteralSynonymCache(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType)
      -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> &;

  auto getCachedRelationshipsByTypes(const RelationshipType &relationshipType,
                                     const EntityType &leftHandEntityType,
                                     const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  void clearCache();

  auto getStoreAndCacheSizes() -> std::vector<int>;

 private:
  auto getCachedEntitiesAndRelationships(
      std::vector<std::vector<std::vector<
          std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>>>
          &store,
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *> &;
};

#endif  // SPA_RELATIONSHIPCACHE_H
