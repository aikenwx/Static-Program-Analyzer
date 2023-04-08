//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "EntityManager.h"
#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

class RelationshipManager {
 private:
  EntityManager *entityManager;

  RelationshipStorage relationshipStorage;

  std::shared_ptr<cfg::CFG> cfg;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static std::vector<Relationship *> emptyRelationshipVector;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static std::vector<Entity *> emptyEntityVector;

 public:
  explicit RelationshipManager(EntityManager *entityManager);

  static auto getEmptyEntityVector() -> std::vector<Entity *> *;

  static auto getEmptyRelationshipVector() -> std::vector<Relationship *> *;

  void storeCFG(std::shared_ptr<cfg::CFG> givenCfg);

  void storeRelationship(std::unique_ptr<Relationship> relationship);

  auto getRelationship(RelationshipKey &key, Entity *leftEntity, Entity *rightEntity) -> Relationship *;

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

  void clearCache();

  // strictly for testing purposes
  auto getStoreAndCacheSizes() -> std::vector<int>;
};

#endif  // SPA_RELATIONSHIPMANAGER_H
