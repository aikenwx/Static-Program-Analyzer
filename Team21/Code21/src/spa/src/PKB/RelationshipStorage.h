//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_RELATIONSHIPSTORAGE_H
#define SPA_RELATIONSHIPSTORAGE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "EntityManager.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

struct RelationshipDoubleSynonymKey : public StorageKey {
 private:
  std::shared_ptr<cfg::CFG> cfg;
  const RelationshipType *relationshipType;
  const EntityType *leftHandEntityType;
  const EntityType *rightHandEntityType;

public:
  RelationshipDoubleSynonymKey(const RelationshipType *relationshipType,
                               const EntityType *leftHandEntityType,
                               const EntityType *rightHandEntityType);
  auto operator==(
      const RelationshipDoubleSynonymKey &otherRelationshipSynonymKey) const
      -> bool;
};

template <>
struct std::hash<RelationshipDoubleSynonymKey> {
  auto operator()(const RelationshipDoubleSynonymKey &relationshipSynonymKey)
      const -> std::size_t;
};

struct RelationshipSynonymLiteralKey : public StorageKey {
 private:
  const RelationshipType *relationshipType;
  const EntityType *leftHandEntityType;
  EntityKey *rightHandEntityKey;
  std::shared_ptr<cfg::CFG> cfg;

 public:
  RelationshipSynonymLiteralKey(const RelationshipType *relationshipType,
                                const EntityType *entityType,
                                EntityKey *entityKey);
  auto operator==(const RelationshipSynonymLiteralKey
                      &otherRelationshipLiteralSynonymKey) const -> bool;
};

template <>
struct std::hash<RelationshipSynonymLiteralKey> {
  auto operator()(
      const RelationshipSynonymLiteralKey &relationshipLiteralSynonymKey) const
      -> std::size_t;
};

struct RelationshipLiteralSynonymKey : public StorageKey {
 private:
  const RelationshipType *relationshipType;
  EntityKey *leftHandEntityKey;
  const EntityType *rightHandEntityType;

 public:
  RelationshipLiteralSynonymKey(const RelationshipType *relationshipType,
                                EntityKey *entityKey,
                                const EntityType *entityType);
  auto operator==(const RelationshipLiteralSynonymKey
                      &otherRelationshipLiteralSynonymKey) const -> bool;
};

template <>
struct std::hash<RelationshipLiteralSynonymKey> {
  auto operator()(
      const RelationshipLiteralSynonymKey &relationshipLiteralSynonymKey) const
      -> std::size_t;
};

class RelationshipStorage {
 private:
    std::unique_ptr<std::vector<Entity*>> placeholderEntityVector;

    std::unique_ptr<std::vector<Relationship *>> placeholderRelationshipVector;

    std::vector<std::unique_ptr<std::vector<Relationship *>>>
      relationshipVectorStoreOwner;


  std::vector<std::unique_ptr<std::vector<Entity *>>> entityVectorStoreOwner;

  std::vector<std::unique_ptr<Relationship>> relationshipStoreOwner;

  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::vector<Relationship *> *>
      relationshipDoubleSynonymStore;

  std::unordered_map<RelationshipSynonymLiteralKey, std::vector<Entity *> *>
      relationshipSynonymLiteralStore;

  std::unordered_map<RelationshipLiteralSynonymKey, std::vector<Entity *> *>
      relationshipLiteralSynonymStore;

  std::unordered_map<RelationshipKey, Relationship *> relationshipStore;

  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::vector<Relationship *> *>
      relationshipDoubleSynonymCache;

  std::unordered_map<
      RelationshipSynonymLiteralKey,
      std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>
      relationshipSynonymLiteralCache;

  std::unordered_map<
      RelationshipLiteralSynonymKey,
      std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>>
      relationshipLiteralSynonymCache;

  std::unordered_map<RelationshipKey, Relationship *> relationshipCache;

  std::vector<std::unique_ptr<Relationship>> relationshipCacheOwner;

  std::vector<std::unique_ptr<std::vector<Relationship *>>>
      relationshipVectorCacheOwner;

  std::vector<std::unique_ptr<std::vector<Entity *>>> entityVectorCacheOwner;

 public:
  RelationshipStorage();

  void storeRelationship(std::unique_ptr<Relationship> relationship);

  void storeInRelationshipDoubleSynonymStore(Relationship *relationship);

  void storeInRelationshipSynonymLiteralStore(Relationship *relationship);

  void storeInRelationshipLiteralSynonymStore(Relationship *relationship);

  void storeInRelationshipOwnerCache(
      std::unique_ptr<Relationship> relationship);

  void storeInRelationshipMapCache(Relationship *relationship);

  void storeInRelationshipLiteralSynonymCaches(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType,
      std::unique_ptr<std::vector<Entity *>> relatedEntities,
      std::unique_ptr<std::vector<Relationship *>> relationships);

  void storeInRelationshipSynonymLiteralCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey,
      std::unique_ptr<std::vector<Entity *>> relatedEntities,
      std::unique_ptr<std::vector<Relationship *>> relationships);

  void storeInRelationshipDoubleSynonymCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType,
      const EntityType &rightHandEntityType,
      std::unique_ptr<std::vector<Relationship *>> relationships);

  auto getRelationshipFromStore(RelationshipKey &key) -> Relationship *;

  auto getRelationshipsByTypesFromStore(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType,
      const EntityType &rightHandEntityType) -> std::vector<Relationship *> *;

  auto getEntitiesFromSynonymLiteralStore(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::vector<Entity *> *;

  auto getEntitiesFromLiteralSynonymStore(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType) -> std::vector<Entity *> *;

  auto getCachedRelationship(RelationshipKey &key) -> Relationship *;

  auto containedInRelationshipCache(RelationshipKey &key) -> bool;

  auto getCachedResultsFromSynonymLiteralCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>;

  auto getCachedResultsFromLiteralSynonymCache(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType)
      -> std::pair<std::vector<Entity *> *, std::vector<Relationship *> *>;

  auto getCachedRelationshipsByTypes(const RelationshipType &relationshipType,
                                     const EntityType &leftHandEntityType,
                                     const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

//  auto getCachedRelationshipsForGivenRelationshipTypeAndRightHandEntityType(
//      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
//      const EntityType &rightHandEntityType) -> std::vector<Relationship *> *;
//
//  auto getCachedRelationshipsForGivenRelationshipTypeAndLeftHandEntityType(
//      const RelationshipType &relationshipType,
//      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
//      -> std::vector<Relationship *> *;

  // strictly for testing purposes
  auto getStoreAndCacheSizes() -> std::vector<int>;

  void clearCache();

  auto generateRelationshipVector() -> std::vector<Relationship *> *;
  auto generateEntityVector() -> std::vector<Entity *> *;
//auto getVectorForSynonymLiteralStore(RelationshipSynonymLiteralKey & relationshipSynonymLiteralKey) -> std::vector<Entity *>*;

// create template for above method, allowing for different types of keys
// to be passed in
template <typename T>
auto getVectorForStore(T & key, std::unordered_map<T, std::vector<Entity *> *> & store) -> std::vector<Entity *>*;


auto getVectorForDoubleSynonymStore(RelationshipDoubleSynonymKey relationshipDoubleSynonymKey) -> std::vector<Relationship *>*;

};

#endif  // SPA_RELATIONSHIPSTORAGE_H
