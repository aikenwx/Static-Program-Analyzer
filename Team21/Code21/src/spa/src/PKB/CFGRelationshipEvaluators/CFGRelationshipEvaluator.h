//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_CFGRELATIONSHIPEVALUATOR_H
#define SPA_CFGRELATIONSHIPEVALUATOR_H

#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

class CFGRelationshipEvaluator {
 protected:
  cfg::CFG* cfg;
  RelationshipStorage* relationshipStorage;
  EntityManager* entityManager;

  virtual auto isValidEntityInput(Entity* entity) -> bool;

  virtual auto isValidEntityTypeInput(const EntityType& entityType) -> bool;

  void populateCache(bool isReverse,
                     std::shared_ptr<Relationship> relationship);

  void initializeCacheGivenEntityAndEntityType(bool isReverse, Entity &statement,
                                               const EntityType &entityType);

  auto getEntitiesFromStore(bool isReverse, Entity &sourceEntity,
                            const EntityType &destinationEntityType)
      -> std::vector<Entity *> *;

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

  virtual auto getRelationshipType() const -> const RelationshipType& = 0;

  virtual auto getEvaluatableEntitiesFromEntityType(
      const EntityType& entityType) -> std::vector<Entity*>*;

 public:
  CFGRelationshipEvaluator(cfg::CFG* cfg,
                           RelationshipStorage* relationshipStorage,
                           EntityManager* entityManager);

  void evaluateAndCacheRelationshipsByGivenEntities(
      Entity* leftEntity, Entity* rightEntity);

  void evaluateAndCacheRelationshipsByEntityTypes(
      const EntityType& leftEntityType, const EntityType& rightEntityType);

  void evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      const EntityType& givenEntityType, Entity* entity, bool isReverse);

 private:
  auto shouldEvaluateRelationshipsByEntityTypesInReverse(
      const EntityType& leftEntityType, const EntityType& rightEntityType)
      -> bool;
};

#endif  // SPA_CFGRELATIONSHIPEVALUATOR_H
