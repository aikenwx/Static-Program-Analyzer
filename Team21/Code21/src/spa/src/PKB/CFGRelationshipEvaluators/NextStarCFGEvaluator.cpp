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

auto NextStarCFGEvaluator::getRelatedBlockStatementPairs(
    std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
    bool isReverse)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> {
  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(cfg, relationshipStorage, entityManager);

  auto results = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>();

  // push all statements from results into stack

  for (auto result : *nextCFGEvaluator.getRelatedBlockStatementPairs(
           sourceBlockStatementPair, isReverse)) {
    statementsToVisit.push(result);
  }

  // bfs
  while (!statementsToVisit.empty()) {
    auto nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    if (visitedStatementNumbers.find(
            nextToVisit->second->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(nextToVisit->second->getStatementNumber());
    results->push_back(nextToVisit);

    auto nextResults =
        nextCFGEvaluator.getRelatedBlockStatementPairs(*nextToVisit, isReverse);

    for (auto nextResult : *nextResults) {
      statementsToVisit.push(nextResult);
    }
  }

  return results;
}

auto NextStarCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                                 Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
  return std::make_shared<NextStarRelationship>(leftStatement, rightStatement);
}
