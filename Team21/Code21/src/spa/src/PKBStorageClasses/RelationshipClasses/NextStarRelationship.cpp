//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarRelationship.h"

auto NextStarRelationshipType::getRelationshipEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                                        RelationshipCache *relationshipCache,
                                                        EntityManager *entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> const {
  return std::make_shared<NextStarCFGEvaluator>(cfg, relationshipStorage, relationshipCache, entityManager);
}

NextStarRelationship::NextStarRelationship(Statement* prevStatement, Statement* nextStatement) : Relationship(&NextStarRelationship::relationshipType, prevStatement, nextStatement) {
}

const NextStarRelationshipType NextStarRelationship::relationshipType = NextStarRelationshipType();

auto NextStarRelationship::getRelationshipTypeStatic() -> const RelationshipType& {
  return NextStarRelationship::relationshipType;
}

auto NextStarRelationship::getRelationshipType() const -> const RelationshipType& {
  return NextStarRelationship::relationshipType;
}
