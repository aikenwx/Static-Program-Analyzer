//
// Created by Aiken Wong on 29/3/23.
//

#ifndef SPA_AFFECTSSTARCFGEVALUATOR_H
#define SPA_AFFECTSSTARCFGEVALUATOR_H

#include "AffectsRelatedCFGEvaluator.h"
class AffectsStarCFGEvaluator : public AffectsRelatedCFGEvaluator {
 public:
  AffectsStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                          EntityManager* entityManager);

  auto getRelationshipType() const -> const RelationshipType& override;

  auto createNewRelationship(Statement* leftStatement,
                             Statement* rightStatement)
      -> std::shared_ptr<Relationship> override;

  auto getRelatedBlockStatementPairs(
      std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
      bool isReverse)
      -> std::shared_ptr<std::vector<
          std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> override;
};

#endif  // SPA_AFFECTSSTARCFGEVALUATOR_H
