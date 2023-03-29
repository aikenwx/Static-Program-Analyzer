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

    auto createNewRelationship(Statement* leftStatement,
                               Statement* rightStatement)
    -> std::shared_ptr<Relationship> override;

    auto getRelatedBlockStatementPairs(
            std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
            bool isReverse)
    -> std::shared_ptr<std::vector<
            std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> override;
};

#endif  // SPA_NEXTSTARCFGEVALUATOR_H
