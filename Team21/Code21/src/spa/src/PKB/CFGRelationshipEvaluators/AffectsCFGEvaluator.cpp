//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsCFGEvaluator.h"

#include <stack>
#include <stdexcept>

#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

AffectsCFGEvaluator::AffectsCFGEvaluator(cfg::CFG* cfg,
                                         RelationshipStorage* relationshipStorage,
                                         EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return AffectsRelationship::getRelationshipTypeStatic();
}

auto AffectsCFGEvaluator::getRelatedBlockStatementPairs(
    std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
    bool isReverse)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> {
  auto* statement = sourceBlockStatementPair.second;
  if (!isValidEntityInput(sourceBlockStatementPair.second)) {
    throw std::invalid_argument(
        "AffectsCFGEvaluator::getRelatedBlockStatementPairs: "
        "sourceBlockStatementPair.second is not a valid entity");
  }

  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  auto* modifiedVariable = getRelationshipStorage()->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                                                       ModifiesRelationship::getRelationshipTypeStatic(),
                                                       statement->getEntityKey(),
                                                       Variable::getEntityTypeStatic())
                               ->at(0);

  auto results = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>();

  // push all statements from results into stack
  auto directRelations = nextCFGEvaluator.getRelatedBlockStatementPairs(
      sourceBlockStatementPair, isReverse);

  for (const auto& result : *directRelations) {
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

    auto usesRelationshipKey = RelationshipKey(
        &UsesRelationship::getRelationshipTypeStatic(),
        &nextToVisit->second->getEntityKey(),
        &modifiedVariable->getEntityKey());

    // we check uses first, as we don't want to skip case where assign statement uses and modifies same variable that was modified by source statement
    // statement is affected by source statement
    if ((getRelationshipStorage()->getRelationship(usesRelationshipKey) != nullptr) && isValidEntityInput(nextToVisit->second)) {
      // only add if its an assign statement
      results->push_back(nextToVisit);
      continue;
    }

    auto modifiesRelationshipKey = RelationshipKey(
        &ModifiesRelationship::getRelationshipTypeStatic(),
        &nextToVisit->second->getEntityKey(),
        &modifiedVariable->getEntityKey());

    // there is a modification, we terminate traversing this path
    if (getRelationshipStorage()->getRelationship(modifiesRelationshipKey) != nullptr) {
      continue;
    }

    auto nextResults =
        nextCFGEvaluator.getRelatedBlockStatementPairs(*nextToVisit, isReverse);

    for (const auto& nextResult : *nextResults) {
      statementsToVisit.push(nextResult);
    }
  }

  return results;
}

auto AffectsCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                                Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
  auto* leftAssignStatement = dynamic_cast<AssignStatement*>(leftStatement);
  auto* rightAssignStatement = dynamic_cast<AssignStatement*>(rightStatement);

  if (leftAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  }
  if (rightAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  }
  return std::make_shared<AffectsRelationship>(leftAssignStatement, rightAssignStatement);
}
