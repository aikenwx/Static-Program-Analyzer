//
// Created by Aiken Wong on 25/3/23.
//

#include "NextStarCFGEvaluator.h"

#include <stack>

#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"

NextStarCFGEvaluator::NextStarCFGEvaluator(
    cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
    EntityManager* entityManager)
    : NextRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextStarCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return NextStarRelationship::getRelationshipTypeStatic();
}

auto NextStarCFGEvaluator::getRelatedStatements(Statement* statement,
                                                bool isReverse)
    -> std::unique_ptr<std::vector<Entity*>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(),
                                    getEntityManager());

  auto results = std::make_unique<std::vector<Entity*>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();

  auto* directRelations =
      nextCFGEvaluator
          .getCachedEntitiesAndRelationships(isReverse, *statement,
                                             Statement::getEntityTypeStatic())
          .first;
  if (directRelations == nullptr) {
    directRelations =
        nextCFGEvaluator
            .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                Statement::getEntityTypeStatic(), statement, isReverse)
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

    visitedStatementNumbers[nextToVisit] = true;
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
        nextCFGEvaluator
            .getCachedEntitiesAndRelationships(isReverse, *nextToVisitStmt,
                                               Statement::getEntityTypeStatic())
            .first;

    if (neighbours == nullptr) {
      neighbours =
          nextCFGEvaluator
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
