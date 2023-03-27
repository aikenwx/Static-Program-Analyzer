//
// Created by Aiken Wong on 25/3/23.
//

#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"

NextCFGEvaluator::NextCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextCFGEvaluator::evaluateAndCacheRelationshipFromLeftStatement(Statement* leftStatement) -> std::vector<Relationship*>* {
  return new std::vector<Relationship*>();
}

auto NextCFGEvaluator::evaluateAndCacheRelationshipFromRightStatement(Statement* rightEntity) -> std::vector<Relationship*>* {
  return new std::vector<Relationship*>();
}

auto NextCFGEvaluator::getRelationshipType() const -> const RelationshipType & {
    return NextRelationship::getRelationshipTypeStatic();
}