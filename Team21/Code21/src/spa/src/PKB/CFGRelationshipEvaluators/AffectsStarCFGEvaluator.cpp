//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsStarCFGEvaluator.h"

#include <stack>
#include <stdexcept>

#include "AffectsCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"

AffectsStarCFGEvaluator::AffectsStarCFGEvaluator(
    cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
    EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsStarCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarCFGEvaluator::getRelatedStatements(Statement* sourceStatement,
                                                   bool isReverse)
    -> std::unique_ptr<std::vector<Entity*>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  AffectsCFGEvaluator affectsCFGEvaluator(getCFG(), getRelationshipStorage(),
                                          getEntityManager());

  auto results = std::make_unique<std::vector<Entity*>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();

  // push all statements from results into stack

  auto* directRelations =
      affectsCFGEvaluator
          .getCachedEntitiesAndRelationships(isReverse, *sourceStatement,
                                             Statement::getEntityTypeStatic())
          .first;

  if (directRelations == nullptr) {
    directRelations =
        affectsCFGEvaluator
            .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                Statement::getEntityTypeStatic(), sourceStatement, isReverse)
            .first;
  }

  for (auto* result : *directRelations) {
    statementNumbersToVisit.push(*result->getEntityKey().getOptionalInt());
  }

  while (!statementNumbersToVisit.empty()) {
    auto nextToVisit = statementNumbersToVisit.top();
    statementNumbersToVisit.pop();

    if (visitedStatementNumbers.at(nextToVisit)) {
      continue;
    }

    visitedStatementNumbers.at(nextToVisit) = true;
    auto* nextToVisitStmt = getEntityManager()->getStmtByNumber(nextToVisit);
    results->push_back(nextToVisitStmt);

    auto* possibleCachedResults =
        this->getCachedEntitiesAndRelationships(
                isReverse, *nextToVisitStmt, Statement::getEntityTypeStatic())
            .first;

    if (possibleCachedResults != nullptr) {
      for (const auto& result : *possibleCachedResults) {
        int stmtNumber = *result->getEntityKey().getOptionalInt();

        if (!visitedStatementNumbers.at(stmtNumber)) {
          visitedStatementNumbers.at(stmtNumber) = true;
          results->push_back(result);
        }
      }
      continue;
    }

    auto* neighbours =
        affectsCFGEvaluator
            .getCachedEntitiesAndRelationships(isReverse, *nextToVisitStmt,
                                               Statement::getEntityTypeStatic())
            .first;

    if (neighbours == nullptr) {
      neighbours =
          affectsCFGEvaluator
              .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                  Statement::getEntityTypeStatic(), nextToVisitStmt, isReverse)
              .first;
    }
    for (auto* neighbour : *neighbours) {
      statementNumbersToVisit.push(*neighbour->getEntityKey().getOptionalInt());
    }
  }

  return results;
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
