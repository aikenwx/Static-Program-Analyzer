//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_CFGRELATIONSHIPEVALUATOR_H
#define SPA_CFGRELATIONSHIPEVALUATOR_H

#include "PKB/RelationshipCache.h"
#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

class CFGRelationshipEvaluator {
 private:
  cfg::CFG* cfg;
  RelationshipStorage* relationshipStorage;
  EntityManager* entityManager;
  RelationshipCache* relationshipCache;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static CacheResult emptyCacheResult;

 protected:
  virtual auto isValidEntityInput(Entity* entity) -> bool;

  virtual auto isValidEntityTypeInput(const EntityType& entityType) -> bool;

  virtual auto createNewRelationship(Entity* leftStatement,
                                     Entity* rightStatement)
      -> std::unique_ptr<Relationship> = 0;

  virtual auto getRelatedStatements(Statement* statement, bool isReverse)
      -> std::unique_ptr<std::vector<Entity*>> = 0;

  [[nodiscard]] virtual auto getRelationshipType() const
      -> const RelationshipType& = 0;

  virtual auto getEvaluableEntitiesFromEntityType(const EntityType& entityType)
      -> std::vector<Entity*>*;

  auto getCFG() -> cfg::CFG*;

  auto getRelationshipStorage() -> RelationshipStorage*;

  auto getEntityManager() -> EntityManager*;

  auto getRelationshipCache() -> RelationshipCache*;

  virtual auto shouldEvaluateRelationshipsByEntityTypesInReverse(
      std::vector<Entity*>* leftEntityVector,
      std::vector<Entity*>* rightEntityVector) -> bool = 0;

  virtual auto shouldSortForDoubleEnityTypeEvaluation() -> bool;

  auto solveTransitiveRelationship(Statement* sourceStatement, bool isReverse, CFGRelationshipEvaluator& baseEvaluator) -> std::unique_ptr<std::vector<Entity*>>;

 public:
  CFGRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                           RelationshipCache* relationshipCache, EntityManager* entityManager);

  auto operator=(const CFGRelationshipEvaluator& cfgRelationshipEvaluator)
      -> CFGRelationshipEvaluator& = default;

  CFGRelationshipEvaluator(
      const CFGRelationshipEvaluator& cfgRelationshipEvaluator) = default;

  auto operator=(CFGRelationshipEvaluator&& cfgRelationshipEvaluator) noexcept
      -> CFGRelationshipEvaluator& = default;

  CFGRelationshipEvaluator(
      CFGRelationshipEvaluator&& cfgRelationshipEvaluator) noexcept = default;

  virtual ~CFGRelationshipEvaluator() = default;

  auto evaluateAndCacheRelationshipsByGivenEntities(
      Entity* leftEntity, Entity* rightEntity) -> Relationship*;

  auto evaluateAndCacheRelationshipsByEntityTypes(
      const EntityType& leftEntityType, const EntityType& rightEntityType) -> std::vector<Relationship*>*;

  auto evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      const EntityType& givenEntityType, Entity* entity, bool isReverse) -> CacheResult&;

  auto getCachedEntitiesAndRelationships(
      bool isReverse, Entity& sourceEntity,
      const EntityType& destinationEntityType)
      -> CacheResult&;
};

#endif  // SPA_CFGRELATIONSHIPEVALUATOR_H
