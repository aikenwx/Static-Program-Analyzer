//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsStarCFGEvaluator.h"

#include <stack>
#include <stdexcept>

#include "AffectsCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"

AffectsStarCFGEvaluator::AffectsStarCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                                                 RelationshipCache* relationshipCache, EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, relationshipCache, entityManager) {}

auto AffectsStarCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarCFGEvaluator::getRelatedStatements(Statement* sourceStatement,
                                                   bool isReverse)
    -> std::unique_ptr<std::vector<Entity*>> {
  AffectsCFGEvaluator affectsCFGEvaluator(getCFG(), getRelationshipStorage(), getRelationshipCache(),
                                          getEntityManager());

  return solveTransitiveRelationship(sourceStatement, isReverse,
                                     affectsCFGEvaluator);
}

auto AffectsStarCFGEvaluator::createNewRelationship(Entity* leftStatement,
                                                    Entity* rightStatement)
    -> std::unique_ptr<Relationship> {
  auto* leftAssignStatement = dynamic_cast<AssignStatement*>(leftStatement);
  auto* rightAssignStatement = dynamic_cast<AssignStatement*>(rightStatement);

  if (leftAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsStarCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  }
  if (rightAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsStarCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  }
  return std::make_unique<AffectsStarRelationship>(leftAssignStatement,
                                                   rightAssignStatement);
}

auto AffectsStarCFGEvaluator::shouldSortForDoubleEnityTypeEvaluation() -> bool {
  return true;
}
