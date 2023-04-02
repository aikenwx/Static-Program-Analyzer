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
    : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto NextStarCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return NextStarRelationship::getRelationshipTypeStatic();
}

auto NextStarCFGEvaluator::getRelatedStatements(
    Statement* sourceStatement,
    bool isReverse)
    -> std::shared_ptr<std::vector<Statement*>> {
  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  auto results = std::make_shared<std::vector<Statement*>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<Statement*>();

  nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      Statement::getEntityTypeStatic(), sourceStatement, isReverse);
  // push all statements from results into stack
  auto directRelations = nextCFGEvaluator.getEntitiesFromStore(isReverse, *sourceStatement, Statement::getEntityTypeStatic());

  for (const auto& result : *directRelations) {
    statementsToVisit.push(dynamic_cast<Statement*>(result));
  }

  while (!statementsToVisit.empty()) {
    auto nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    if (visitedStatementNumbers.find(
            nextToVisit->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(nextToVisit->getStatementNumber());
    results->push_back(nextToVisit);

    nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        Statement::getEntityTypeStatic(), nextToVisit, isReverse);

    auto nextResults = nextCFGEvaluator.getEntitiesFromStore(isReverse, *nextToVisit, Statement::getEntityTypeStatic());

    for (const auto& nextResult : *nextResults) {
      statementsToVisit.push(dynamic_cast<Statement*>(nextResult));
    }
  }

  return results;
}

auto NextStarCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                                 Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
  return std::make_shared<NextStarRelationship>(leftStatement, rightStatement);
}
