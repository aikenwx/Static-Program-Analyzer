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

auto NextStarCFGEvaluator::getRelatedStatementNumbers(int sourceStatementNumber,
                                                      bool isReverse)
    -> std::unique_ptr<std::vector<int>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(),
                                    getEntityManager());

  auto results = std::make_unique<std::vector<int>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();

  auto directRelations = nextCFGEvaluator.getRelatedStatementNumbers(
      sourceStatementNumber, isReverse);
  for (const auto result : *directRelations) {
    statementNumbersToVisit.push(result);
  }

  while (!statementNumbersToVisit.empty()) {
    auto nextToVisit = statementNumbersToVisit.top();
    statementNumbersToVisit.pop();

    if (visitedStatementNumbers.at(nextToVisit)) {
      continue;
    }

    visitedStatementNumbers[nextToVisit] = true;
    results->push_back(nextToVisit);

    auto nextResults =
        nextCFGEvaluator.getRelatedStatementNumbers(nextToVisit, isReverse);

    for (const auto nextResult : *nextResults) {
      statementNumbersToVisit.push(nextResult);
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
