//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsStarCFGEvaluator.h"

#include <stack>
#include <stdexcept>
#include <unordered_map>

#include "AffectsCFGEvaluator.h"
#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"

AffectsStarCFGEvaluator::AffectsStarCFGEvaluator(cfg::CFG* cfg,
                                                 RelationshipStorage* relationshipStorage,
                                                 EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsStarCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarCFGEvaluator::getRelatedStatements(
    Statement* sourceStatement,
    bool isReverse)
    -> std::shared_ptr<std::vector<Statement*>> {
  if (!isValidEntityInput(sourceStatement)) {
    throw std::invalid_argument(
        "AffectsCFGEvaluator::getRelatedStatements: "
        "sourceBlockStatementPair.second is not a valid entity");
  }

  auto visitedStatementNumbers = std::unordered_set<int>();

  AffectsCFGEvaluator affectsCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  auto results = std::make_shared<std::vector<Statement*>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<Statement*>();

  // push all statements from results into stack

  affectsCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      Statement::getEntityTypeStatic(), sourceStatement, isReverse);

  auto* directRelations = affectsCFGEvaluator.getEntitiesFromStore(isReverse,
                                                                   *sourceStatement, Statement::getEntityTypeStatic());

  for (const auto& result : *directRelations) {
    statementsToVisit.push(dynamic_cast<Statement*>(result));
  }

  while (!statementsToVisit.empty()) {
    auto* nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    if (visitedStatementNumbers.find(
            nextToVisit->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(nextToVisit->getStatementNumber());
    results->push_back(nextToVisit);

    affectsCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        Statement::getEntityTypeStatic(), nextToVisit, isReverse);
    auto* nextResults = affectsCFGEvaluator.getEntitiesFromStore(isReverse,
                                                                 *nextToVisit, Statement::getEntityTypeStatic());

    for (const auto& nextResult : *nextResults) {
      statementsToVisit.push(dynamic_cast<Statement*>(nextResult));
    }
  }

  return results;
}

auto AffectsStarCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                                    Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
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
  return std::make_shared<AffectsStarRelationship>(leftAssignStatement, rightAssignStatement);
}
