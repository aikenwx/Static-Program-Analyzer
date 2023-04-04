//
// Created by Aiken Wong on 25/3/23.
//

#include "NextRelationship.h"
#include "PKB/CFGRelationshipEvaluators/NextCFGEvaluator.h"


auto NextRelationshipType::getRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> {
    return std::make_shared<NextCFGEvaluator>(cfg, relationshipStorage, entityManager);
}

NextRelationship::NextRelationship(Statement* prevStatement, Statement* nextStatement) : Relationship(&NextRelationship::relationshipType, prevStatement, nextStatement) {
}

const NextRelationshipType NextRelationship::relationshipType = NextRelationshipType();

auto NextRelationship::getRelationshipTypeStatic() -> const RelationshipType& {
    auto test = NextRelationship::relationshipType;
  return NextRelationship::relationshipType;
}

auto NextRelationship::getRelationshipType() const -> const RelationshipType& {
  return NextRelationship::relationshipType;
}
