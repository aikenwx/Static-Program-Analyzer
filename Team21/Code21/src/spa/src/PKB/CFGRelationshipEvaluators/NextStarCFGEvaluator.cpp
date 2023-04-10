//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarCFGEvaluator.h"

#include <stack>

#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

NextStarCFGEvaluator::NextStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                                           RelationshipCache* relationshipCache,
                                           EntityManager* entityManager)
    : NextRelatedCFGEvaluator(cfg, relationshipStorage, relationshipCache, entityManager) {}

auto NextStarCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return NextStarRelationship::getRelationshipTypeStatic();
}

auto NextStarCFGEvaluator::getRelatedStatements(Statement* statement,
                                                bool isReverse)
    -> std::unique_ptr<std::vector<Entity*>> {
  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getRelationshipCache(),
                                    getEntityManager());

  return solveTransitiveRelationship(statement, isReverse,
                                     nextCFGEvaluator);
}

auto NextStarCFGEvaluator::createNewRelationship(Entity* leftStatement,
                                                 Entity* rightStatement)
    -> std::unique_ptr<Relationship> {
  return std::make_unique<NextStarRelationship>(
      dynamic_cast<Statement*>(leftStatement),
      dynamic_cast<Statement*>(rightStatement));
}

auto NextStarCFGEvaluator::shouldSortForDoubleEnityTypeEvaluation() -> bool {
  return true;
}
