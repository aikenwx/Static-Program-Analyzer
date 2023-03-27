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

    auto operator()(Statement* statement, bool isReverse) -> std::vector<Entity*>* override;
};

#endif  // SPA_NEXTSTARCFGEVALUATOR_H
