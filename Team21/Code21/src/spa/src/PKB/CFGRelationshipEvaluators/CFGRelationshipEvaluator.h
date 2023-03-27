//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_CFGRELATIONSHIPEVALUATOR_H
#define SPA_CFGRELATIONSHIPEVALUATOR_H

#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"


struct BlockStatementPairIterator {
  virtual auto nextBlockStatementPairs(std::pair<cfg::Block*, int> & currentBlockStatementPair)
      -> std::shared_ptr<std::vector<std::shared_ptr<std::pair<cfg::Block*, int>>>> = 0;

};

struct ForwardBlockStatementPairIterator : public BlockStatementPairIterator {
  auto nextBlockStatementPairs(std::pair<cfg::Block*, int> & currentBlockStatementPair)
      -> std::shared_ptr<std::vector<std::shared_ptr<std::pair<cfg::Block*, int>>>> override;
};

struct ReverseBlockStatementPairIterator : public BlockStatementPairIterator {
  auto nextBlockStatementPairs(std::pair<cfg::Block*, int> & currentBlockStatementPair)
      -> std::shared_ptr<std::vector<std::shared_ptr<std::pair<cfg::Block*, int>>>> override;
};

class CFGRelationshipEvaluator {
 protected:
  cfg::CFG* cfg;
  RelationshipStorage* relationshipStorage;
  EntityManager* entityManager;

 public:
  CFGRelationshipEvaluator(cfg::CFG* cfg,
                           RelationshipStorage* relationshipStorage,
                           EntityManager* entityManager);


  virtual auto getRelationshipType() const -> const RelationshipType& = 0;

    // if relationships is evaluated in reverse, then given a statement, we find all statements that the
    // given statement has a reverse relationship with
    // e.g. given Next relationship, statement = 1, isReverse = true, then we evaluate all statements s
    // such that Next(s, 1) is true
    // otherwise if isReverse = false, we evaluate all statements s such that Next(1, s) is true
  virtual auto operator()(Statement* statement, bool isReverse)
      -> std::vector<Entity*>* = 0;

  void evaluateAndCacheRelationshipsByEntities(Entity* leftEntity,
                                               Entity* rightEntity);

  void evaluateAndCacheRelationshipsByEntityTypes(
      const EntityType& leftEntityType, const EntityType& rightEntityType);

  void evaluateAndCacheRelationshipsByLeftEntityTypeAndRightEntity(
      const EntityType& leftEntityType, Entity* rightEntity);

  void evaluateAndCacheRelationshipsByLeftEntityAndRightEntityType(
      Entity* leftEntity, const EntityType& rightEntityType);
};

#endif  // SPA_CFGRELATIONSHIPEVALUATOR_H
