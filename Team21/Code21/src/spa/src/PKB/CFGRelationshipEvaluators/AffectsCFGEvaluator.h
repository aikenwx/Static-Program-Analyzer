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

 public:
  AffectsCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, RelationshipCache* relationshipCache,
                      EntityManager* entityManager);

  [[nodiscard]] auto getRelationshipType() const
      -> const RelationshipType& override;

  auto createNewRelationship(Entity* leftStatement, Entity* rightStatement)
      -> std::unique_ptr<Relationship> override;

  auto getRelatedStatements(Statement* sourceStatement, bool isReverse)
      -> std::unique_ptr<std::vector<Entity*>> override;

 private:
  auto getRelatedStatementsInForwardsEvaluation(Statement* sourceStatement)
      -> std::unique_ptr<std::vector<Entity*>>;

  auto getRelatedStatementsInReverseEvaluation(Statement* sourceStatement)
      -> std::unique_ptr<std::vector<Entity*>>;

  void initializeForwardsEvaluation(Statement* sourceStatement);

  auto visitInForwardsEvaluation(Statement* statement,
                                 std::vector<Statement*>* partialResults)
      -> bool;
};

#endif  // SPA_AFFECTSCFGEVALUATOR_H
