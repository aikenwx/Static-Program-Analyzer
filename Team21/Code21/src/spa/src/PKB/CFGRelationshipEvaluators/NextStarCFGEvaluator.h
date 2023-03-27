//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTSTARCFGEVALUATOR_H
#define SPA_NEXTSTARCFGEVALUATOR_H

#include "CFGRelationshipEvaluator.h"
class NextStarCFGEvaluator : public CFGRelationshipEvaluator {
 public:

  NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager);

    auto getRelationshipType() const -> const RelationshipType & override;

  auto evaluateAndCacheRelationshipFromLeftStatement(Statement* leftStatement) -> std::vector<Relationship*>* override;

  auto evaluateAndCacheRelationshipFromRightStatement(Statement* rightEntity) -> std::vector<Relationship*>* override;
};

#endif  // SPA_NEXTSTARCFGEVALUATOR_H
