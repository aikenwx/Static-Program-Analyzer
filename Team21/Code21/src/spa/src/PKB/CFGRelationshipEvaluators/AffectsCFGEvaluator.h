//
// Created by Aiken Wong on 29/3/23.
//

#ifndef SPA_AFFECTSCFGEVALUATOR_H
#define SPA_AFFECTSCFGEVALUATOR_H

#include "AffectsRelatedCFGEvaluator.h"
#include "CFGRelationshipEvaluator.h"

class AffectsCFGEvaluator : public AffectsRelatedCFGEvaluator {
 private:
  Entity* modifiedEntityFromSource;

  std::shared_ptr<std::unordered_map<std::string, Entity*>>
      currentUnusedVariables;

 public:
  AffectsCFGEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage, RelationshipCache *relationshipCache,
                      EntityManager *entityManager);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType& override;

  auto createNewRelationship(Entity* leftStatement, Entity* rightStatement)
      -> std::unique_ptr<Relationship> override;

  auto getRelatedStatements(Statement* sourceStatement, bool isReverse)
      -> std::unique_ptr<std::vector<Entity*>> override;

 private:
  void initializeForwardsEvaluation(Statement* sourceStatement);

  void initializeReverseEvaluation(Statement* sourceStatement);

  // returns true if evaluation results in early termination of path traversal
  auto visitInForwardsEvaluation(Statement* statement,
                                 std::vector<Statement*>* partialResults)
      -> bool;

  auto visitInReverseEvaluation(Statement* visitedStatement,
                                std::vector<Statement*>* partialResults)
      -> bool;
};

#endif  // SPA_AFFECTSCFGEVALUATOR_H
