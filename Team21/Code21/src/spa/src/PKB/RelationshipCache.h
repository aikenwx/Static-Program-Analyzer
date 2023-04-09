//
// Created by Aiken Wong on 9/4/23.
//

#ifndef SPA_RELATIONSHIPCACHE_H
#define SPA_RELATIONSHIPCACHE_H

#include "RelationshipStorage.h"
struct CacheResult {
  CacheResult();
  CacheResult(std::vector<Entity *> *first, std::vector<Relationship *> *second, bool areResultsIndividuallyCached);

  std::vector<Entity *> *first;
  std::vector<Relationship *> *second;
  bool areRelationshipsIndividuallyCached;
};

class RelationshipCache {
 private:
  EntityManager *entityManager;

  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::vector<Relationship *> *>
      relationshipDoubleSynonymCache;

  std::vector<std::unique_ptr<Relationship>> relationshipCacheOwner;

  std::vector<std::unique_ptr<std::vector<Relationship *>>>
      relationshipVectorCacheOwner;

  std::vector<std::unique_ptr<std::vector<Entity *>>> entityVectorCacheOwner;

  // relationshipType -> stmttype -> stmt
  std::vector<std::vector<std::vector<CacheResult>>>
      relationshipLiteralSynonymCache;

  // relationshipType -> stmttype -> stmt
  std::vector<std::vector<std::vector<CacheResult>>>
      relationshipSynonymLiteralCache;

  // relationshipType -> stmt -> stmt -> relationship
  std::vector<std::vector<std::vector<Relationship *>>>
      relationshipMapCache;

 public:
  explicit RelationshipCache(EntityManager *entityManager);

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

  auto getCachedResultsFromSynonymLiteralCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> CacheResult &;

  auto getCachedResultsFromLiteralSynonymCache(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType)
      -> CacheResult &;

  auto getCachedRelationshipsByTypes(const RelationshipType &relationshipType,
                                     const EntityType &leftHandEntityType,
                                     const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  void clearCache();

  auto getStoreAndCacheSizes() -> std::vector<int>;

 private:
  auto getCachedEntitiesAndRelationships(
      std::vector<std::vector<std::vector<CacheResult>>>
          &store,
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> CacheResult &;
};

#endif  // SPA_RELATIONSHIPCACHE_H
