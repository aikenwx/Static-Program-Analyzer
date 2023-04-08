//
// Created by Aiken Wong on 6/4/23.
//

#include "NextRelatedCFGEvaluator.h"

NextRelatedCFGEvaluator::NextRelatedCFGEvaluator(
    cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
    EntityManager *entityManager)
    : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextRelatedCFGEvaluator::shouldEvaluateRelationshipsByEntityTypesInReverse(
    std::vector<Entity *> *leftEntityVector,
    std::vector<Entity *> *rightEntityVector) -> bool {
  return rightEntityVector->size() < leftEntityVector->size();
}
