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
  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::shared_ptr<std::vector<Relationship *>>>
      relationshipDoubleSynonymStore;

  std::unordered_map<RelationshipSynonymLiteralKey,
                     std::shared_ptr<std::vector<Entity *>>>
      relationshipSynonymLiteralStore;

  std::unordered_map<RelationshipLiteralSynonymKey,
                     std::shared_ptr<std::vector<Entity *>>>
      relationshipLiteralSynonymStore;

  std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>>
      relationshipStore;

  std::unordered_map<RelationshipDoubleSynonymKey,
                     std::shared_ptr<std::vector<Relationship *>>>
      relationshipDoubleSynonymCache;

  std::unordered_map<RelationshipSynonymLiteralKey,
                     std::shared_ptr<std::vector<Entity *>>>
      relationshipSynonymLiteralCache;

  std::unordered_map<RelationshipLiteralSynonymKey,
                     std::shared_ptr<std::vector<Entity *>>>
      relationshipLiteralSynonymCache;

  std::unordered_map<RelationshipSynonymLiteralKey,
                     std::shared_ptr<std::vector<Relationship *>>>
      relationshipVectorSynonymLiteralCache;

  std::unordered_map<RelationshipLiteralSynonymKey,
                     std::shared_ptr<std::vector<Relationship *>>>
      relationshipVectorLiteralSynonymCache;

  std::unordered_map<RelationshipKey, Relationship *> relationshipCache;

  std::vector<std::shared_ptr<Relationship>> relationshipOwnerCache;

 public:
  RelationshipStorage();

  void storeRelationship(const std::shared_ptr<Relationship> &relationship);

  auto tryStoreRelationshipOnlyInRelationshipStore(
      const std::shared_ptr<Relationship> &relationship, bool useCache) -> bool;

  void storeInSpecifiedRelationshipDoubleSynonymStore(
      Relationship *relationship, const RelationshipDoubleSynonymKey &key,
      bool useCache);

  void storeInRelationshipDoubleSynonymStore(Relationship *relationship);

  void storeInRelationshipSynonymLiteralStore(Relationship *relationship,
                                              bool useCache);

  void storeInRelationshipLiteralSynonymStore(Relationship *relationship,
                                              bool useCache);

  void storeInRelationshipOwnerCache(
      std::shared_ptr<Relationship> relationship);

 void storeInRelationshipMapCache(Relationship *relationship);

  void storeInRelationshipLiteralSynonymCache(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType,
      std::shared_ptr<std::vector<Entity *>> relatedEntities);

  void storeInRelationshipSynonymLiteralCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey,
      std::shared_ptr<std::vector<Entity *>> relatedEntities);

  void storeInRelationshipLiteralSynonymCache(
          const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
          const EntityType &rightHandEntityType,
          std::shared_ptr<std::vector<Relationship *>>
          relationships);

  void storeInRelationshipSynonymLiteralCache(
          const RelationshipType &relationshipType,
          const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey,
          std::shared_ptr<std::vector<Relationship *>>
          relationships);

  void storeInRelationshipDoubleSynonymCache(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType,
      const EntityType &rightHandEntityType,
      std::shared_ptr<std::vector<Relationship *>> relationships);

  auto getCachedRelationship(RelationshipKey &key) -> Relationship *;

  auto containedInRelationshipCache(RelationshipKey &key) -> bool;

  auto getRelationship(RelationshipKey &key) -> Relationship *;

  auto getCachedEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::vector<Entity *> *;

  auto getCachedEntitiesForGivenRelationshipTypeAndRightHandEntityType(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType) -> std::vector<Entity *> *;

  auto getCachedRelationshipsByTypes(const RelationshipType &relationshipType,
                                     const EntityType &leftHandEntityType,
                                     const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  auto getCachedRelationshipsForGivenRelationshipTypeAndRightHandEntityType(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  auto getCachedRelationshipsForGivenRelationshipTypeAndLeftHandEntityType(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::vector<Relationship *> *;

  auto getRelationshipsByTypes(const RelationshipType &relationshipType,
                               const EntityType &leftHandEntityType,
                               const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  auto getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
      const RelationshipType &relationshipType,
      const EntityType &leftHandEntityType, EntityKey &rightHandEntityKey)
      -> std::vector<Entity *> *;
  auto getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
      const RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType) -> std::vector<Entity *> *;

  void initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(
      const RelationshipDoubleSynonymKey &relationshipSynonymKey,
      bool useCache);

  void initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(
      RelationshipLiteralSynonymKey relationshipLiteralSynonymKey,
      bool useCache);

  void initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(
      const RelationshipSynonymLiteralKey &relationshipSynonymLiteralKey,
      bool useCache);

  // strictly for testing purposes
  auto getStoreAndCacheSizes() -> std::vector<int>;

  void clearCache();
};

#endif  // SPA_RELATIONSHIPSTORAGE_H
