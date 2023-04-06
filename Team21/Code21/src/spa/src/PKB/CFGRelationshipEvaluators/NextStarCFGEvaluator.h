//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTSTARCFGEVALUATOR_H
#define SPA_NEXTSTARCFGEVALUATOR_H

#include "NextRelatedCFGEvaluator.h"
class NextStarCFGEvaluator : public NextRelatedCFGEvaluator {
 public:

  NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager);

    [[nodiscard]] auto getRelationshipType() const -> const RelationshipType & override;

    auto createNewRelationship(Entity *leftStatement,
                               Entity *rightStatement)
    -> std::shared_ptr<Relationship> override;

    auto getRelatedStatementNumbers(
            int sourceStatementNumber,
            bool isReverse)
    -> std::shared_ptr<std::vector<int>> override;
};

#endif  // SPA_NEXTSTARCFGEVALUATOR_H
