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

auto AffectsCFGEvaluator::getRelatedStatements(
    Statement* sourceStatement,
    bool isReverse)
    -> std::shared_ptr<std::vector<Statement*>> {
  if (!isValidEntityInput(sourceStatement)) {
    throw std::invalid_argument(
        "AffectsCFGEvaluator::getRelatedStatements: "
        "sourceBlockStatementPair.second is not a valid entity");
  }

  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getEntityManager());

  auto* modifiedVariable = getRelationshipStorage()->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                                                       ModifiesRelationship::getRelationshipTypeStatic(),
                                                       sourceStatement->getEntityKey(),
                                                       Variable::getEntityTypeStatic())
                               ->at(0);

  auto results = std::make_shared<std::vector<Statement*>>();

  // create stack of statements to visit
  auto statementsToVisit = std::stack<Statement*>();

  // push all statements from results into stack

  nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      Statement::getEntityTypeStatic(), sourceStatement, isReverse);

  auto *directRelations = nextCFGEvaluator.getEntitiesFromStore(isReverse,
                                                               *sourceStatement, Statement::getEntityTypeStatic());

  for (const auto& result : *directRelations) {
    statementsToVisit.push(dynamic_cast<Statement*>(result));
  }

  // bfs
  while (!statementsToVisit.empty()) {
    auto *nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    if (visitedStatementNumbers.find(
            nextToVisit->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(nextToVisit->getStatementNumber());

    auto usesRelationshipKey = RelationshipKey(
        &UsesRelationship::getRelationshipTypeStatic(),
        &nextToVisit->getEntityKey(),
        &modifiedVariable->getEntityKey());

    // we check uses first, as we don't want to skip case where assign statement uses and modifies same variable that was modified by source statement
    // statement is affected by source statement
    if ((getRelationshipStorage()->getRelationship(usesRelationshipKey) != nullptr) && isValidEntityInput(nextToVisit)) {
      // only add if its an assign statement
      results->push_back(nextToVisit);
    }

    if (isAllowedModifier(nextToVisit)) {
      auto modifiesRelationshipKey = RelationshipKey(
          &ModifiesRelationship::getRelationshipTypeStatic(),
          &nextToVisit->getEntityKey(),
          &modifiedVariable->getEntityKey());

      // there is a modification, we terminate traversing this path
      if (getRelationshipStorage()->getRelationship(modifiesRelationshipKey) != nullptr) {
        continue;
      }
    }

    nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        Statement::getEntityTypeStatic(), nextToVisit, isReverse);

    auto *nextResults = nextCFGEvaluator.getEntitiesFromStore(isReverse,
                                                             *nextToVisit, Statement::getEntityTypeStatic());

    for (const auto& nextResult : *nextResults) {
      statementsToVisit.push(dynamic_cast<Statement*>(nextResult));
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
