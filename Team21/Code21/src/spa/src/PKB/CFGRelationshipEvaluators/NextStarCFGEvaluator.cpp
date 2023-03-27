//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

NextStarCFGEvaluator::NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextStarCFGEvaluator::evaluateAndCacheRelationshipFromLeftStatement(Statement* leftStatement) -> std::vector<Relationship*>* {
  return new std::vector<Relationship*>();
}

auto NextStarCFGEvaluator::evaluateAndCacheRelationshipFromRightStatement(Statement* rightEntity) -> std::vector<Relationship*>* {
  return new std::vector<Relationship*>();
}

auto NextStarCFGEvaluator::getRelationshipType() const -> const RelationshipType & {
    return NextStarRelationship::getRelationshipTypeStatic();
}