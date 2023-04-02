//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsStarCFGEvaluator.h"

#include <stack>
#include <stdexcept>
#include <unordered_map>

#include "AffectsCFGEvaluator.h"
#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

AffectsStarCFGEvaluator::AffectsStarCFGEvaluator(cfg::CFG* cfg,
                                                 RelationshipStorage* relationshipStorage,
                                                 EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsStarCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarCFGEvaluator::getRelatedBlockStatementPairs(
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

  // auto visitedStatementNumbers = std::unordered_set<int>();

  // NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  // auto* initialModifiedVariable = getRelationshipStorage()->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
  //                                                             ModifiesRelationship::getRelationshipTypeStatic(),
  //                                                             statement->getEntityKey(),
  //                                                             Variable::getEntityTypeStatic())
  //                                     ->at(0);

  // auto results = std::make_shared<
  //     std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  // // create stack of statements to visit
  // auto statementsToVisit =
  //     std::stack<std::shared_ptr<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>>();

  // // push all statements from results into stack
  // auto directRelations = nextCFGEvaluator.getRelatedBlockStatementPairs(
  //     sourceBlockStatementPair, isReverse);

  // for (const auto& result : *directRelations) {
  //   statementsToVisit.push(std::make_shared<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>(result, initialModifiedVariable));
  // }

  // while (!statementsToVisit.empty()) {
  //   auto nextToVisit = statementsToVisit.top();
  //   statementsToVisit.pop();

  //   auto nextPair = nextToVisit->first;
  //   auto* nextStatementToVisit = nextToVisit->first->second;

  //   auto* modifiedVariable = nextToVisit->second;
  //   if (visitedStatementNumbers.find(
  //           nextStatementToVisit->getStatementNumber()) !=
  //       visitedStatementNumbers.end()) {
  //     continue;
  //   }

  //   visitedStatementNumbers.insert(nextStatementToVisit->getStatementNumber());

  //   auto usesRelationshipKey = RelationshipKey(
  //       &UsesRelationship::getRelationshipTypeStatic(),
  //       &nextStatementToVisit->getEntityKey(),
  //       &modifiedVariable->getEntityKey());

  //   auto modifiesRelationshipKey = RelationshipKey(
  //       &ModifiesRelationship::getRelationshipTypeStatic(),
  //       &nextStatementToVisit->getEntityKey(),
  //       &modifiedVariable->getEntityKey());

  //   // we check uses first, as we don't want to skip case where assign statement uses and modifies same variable that was modified by source statement
  //   // statement is affected by source statement
  //   if ((getRelationshipStorage()->getRelationship(usesRelationshipKey) != nullptr) && isValidEntityInput(nextStatementToVisit)) {
  //     // only add if its an assign statement
  //     results->push_back(nextPair);
  //     // get new modified variable
  //     modifiedVariable = getRelationshipStorage()->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
  //                                                    ModifiesRelationship::getRelationshipTypeStatic(),
  //                                                    nextStatementToVisit->getEntityKey(),
  //                                                    Variable::getEntityTypeStatic())
  //                            ->at(0);

  //   } else if (getRelationshipStorage()->getRelationship(modifiesRelationshipKey) != nullptr) {
  //     // there is a modification by non assign statement, we terminate traversing this path
  //     continue;
  //   }

  //   auto nextResults =
  //       nextCFGEvaluator.getRelatedBlockStatementPairs(*nextPair, isReverse);

  //   for (const auto& nextResult : *nextResults) {
  //     statementsToVisit.push(std::make_shared<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>(nextResult, modifiedVariable));
  //   }
  // }

  // return results;

  auto visitedStatementNumbers = std::unordered_set<int>();

  AffectsCFGEvaluator affectsCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  auto results = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>();

  // push all statements from results into stack
  auto directRelations = affectsCFGEvaluator.getRelatedBlockStatementPairs(
      sourceBlockStatementPair, isReverse);

  for (const auto& result : *directRelations) {
    statementsToVisit.push(result);
  }

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

    auto tempVector = std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>();

    auto nextResults = affectsCFGEvaluator.getRelatedBlockStatementPairs(
        *nextToVisit, isReverse);

    for (const auto& nextResult : *nextResults) {
      statementsToVisit.push(nextResult);
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
