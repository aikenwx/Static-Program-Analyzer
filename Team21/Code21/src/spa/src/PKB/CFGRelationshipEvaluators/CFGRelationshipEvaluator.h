//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_CFGRELATIONSHIPEVALUATOR_H
#define SPA_CFGRELATIONSHIPEVALUATOR_H

#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

class CFGRelationshipEvaluator {
 private:
  cfg::CFG* cfg;
  RelationshipStorage* relationshipStorage;
  EntityManager* entityManager;

 protected:
  virtual auto isValidEntityInput(Entity* entity) -> bool;

  virtual auto isValidEntityTypeInput(const EntityType& entityType) -> bool;

  auto generateStatementBlockPair(Statement* statement)
      -> std::shared_ptr<std::pair<cfg::Block*, Statement*>>;

  virtual auto createNewRelationship(Statement* leftStatement,
                                     Statement* rightStatement)
      -> std::shared_ptr<Relationship> = 0;

  // for next, should be O(1), assuming CFG's getNextBlock is O(1)
  // for next*/affect/affect* should be O(N) where N is the number of statements
  // within the containing procedure
  virtual auto getRelatedBlockStatementPairs(
      std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
      bool isReverse)
      -> std::shared_ptr<
          std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> = 0;

  [[nodiscard]] virtual auto getRelationshipType() const -> const RelationshipType& = 0;

  virtual auto getEvaluableEntitiesFromEntityType(
      const EntityType& entityType) -> std::vector<Entity*>*;

  auto getCFG() -> cfg::CFG*;

  auto getRelationshipStorage() -> RelationshipStorage*;

  auto getEntityManager() -> EntityManager*;

 public:
  CFGRelationshipEvaluator(cfg::CFG* cfg,
                           RelationshipStorage* relationshipStorage,
                           EntityManager* entityManager);

  auto operator=(const CFGRelationshipEvaluator& cfgRelationshipEvaluator) -> CFGRelationshipEvaluator& = default;

  CFGRelationshipEvaluator(const CFGRelationshipEvaluator& cfgRelationshipEvaluator) = default;

  auto operator=(CFGRelationshipEvaluator&& cfgRelationshipEvaluator) noexcept -> CFGRelationshipEvaluator& = default;

  CFGRelationshipEvaluator(CFGRelationshipEvaluator&& cfgRelationshipEvaluator) noexcept = default;

  virtual ~CFGRelationshipEvaluator() = default;

  void evaluateAndCacheRelationshipsByGivenEntities(
      Entity* leftEntity, Entity* rightEntity);

  void evaluateAndCacheRelationshipsByEntityTypes(
      const EntityType& leftEntityType, const EntityType& rightEntityType);

  void evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      const EntityType& givenEntityType, Entity* entity, bool isReverse);

  void populateCache(bool isReverse,
                     const std::shared_ptr<Relationship>& relationship);

  void initializeCacheGivenEntityAndEntityType(bool isReverse, Entity& statement,
                                               const EntityType& entityType);
  auto getEntitiesFromStore(bool isReverse, Entity& sourceEntity,
                            const EntityType& destinationEntityType)
      -> std::vector<Entity*>*;

 private:
  auto shouldEvaluateRelationshipsByEntityTypesInReverse(
      const EntityType& leftEntityType, const EntityType& rightEntityType)
      -> bool;
};

#endif  // SPA_CFGRELATIONSHIPEVALUATOR_H
