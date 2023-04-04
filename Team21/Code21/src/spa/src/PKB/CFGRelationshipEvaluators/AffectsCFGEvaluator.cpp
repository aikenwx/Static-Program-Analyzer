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

AffectsCFGEvaluator::AffectsCFGEvaluator(
    cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
    EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, entityManager),
      modifiedEntityFromSource(nullptr),
      usedEntitiesFromSource(nullptr) {}

auto AffectsCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return AffectsRelationship::getRelationshipTypeStatic();
}

auto AffectsCFGEvaluator::getRelatedStatements(Statement* sourceStatement,
                                               bool isReverse)
    -> std::shared_ptr<std::vector<Statement*>> {
  if (!isValidEntityInput(sourceStatement)) {
    throw std::invalid_argument(
        "AffectsCFGEvaluator::getRelatedStatements: "
        "sourceBlockStatementPair.second is not a valid entity");
  }

  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(),
                                    getEntityManager());

  isReverse ? initializeReverseEvaluation(sourceStatement)
            : initializeForwardsEvaluation(sourceStatement);

  auto results = std::make_shared<std::vector<Statement*>>();

  // create stack of statements to visit
  auto statementsToVisit = std::stack<Statement*>();

  // push all statements from results into stack

  nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
      Statement::getEntityTypeStatic(), sourceStatement, isReverse);

  auto* directRelations = nextCFGEvaluator.getEntitiesFromStore(
      isReverse, *sourceStatement, Statement::getEntityTypeStatic());

  for (const auto& result : *directRelations) {
    statementsToVisit.push(dynamic_cast<Statement*>(result));
  }

  while (!statementsToVisit.empty()) {
    auto* nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    if (visitedStatementNumbers.find(nextToVisit->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(nextToVisit->getStatementNumber());

    if (isReverse ? visitInReverseEvaluation(nextToVisit, results.get())
                  : visitInForwardsEvaluation(nextToVisit, results.get())) {
      continue;
    }

    nextCFGEvaluator.evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
        Statement::getEntityTypeStatic(), nextToVisit, isReverse);

    auto* neighbours = nextCFGEvaluator.getEntitiesFromStore(
        isReverse, *nextToVisit, Statement::getEntityTypeStatic());

    for (const auto& neighbour : *neighbours) {
      statementsToVisit.push(dynamic_cast<Statement*>(neighbour));
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
  return std::make_shared<AffectsRelationship>(leftAssignStatement,
                                               rightAssignStatement);
}

void AffectsCFGEvaluator::initializeForwardsEvaluation(
    Statement* sourceStatement) {
  // assign statements only modify one variable, so we can just get the first
  // one
  modifiedEntityFromSource =
      getRelationshipStorage()
          ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
              ModifiesRelationship::getRelationshipTypeStatic(),
              sourceStatement->getEntityKey(), Variable::getEntityTypeStatic())
          ->at(0);
}

void AffectsCFGEvaluator::initializeReverseEvaluation(
    Statement* sourceStatement) {
  auto* usedEntitiesFromSourceVector =
      getRelationshipStorage()
          ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
              UsesRelationship::getRelationshipTypeStatic(),
              sourceStatement->getEntityKey(), Variable::getEntityTypeStatic());

  usedEntitiesFromSource =
      std::make_unique<std::unordered_map<EntityKey, Entity*>>();

  for (const auto& usedEntity : *usedEntitiesFromSourceVector) {
    usedEntitiesFromSource->insert({usedEntity->getEntityKey(), usedEntity});
  }
}

auto AffectsCFGEvaluator::visitInForwardsEvaluation(
    Statement* visitedStatement, std::vector<Statement*>* partialResults)
    -> bool {
  auto usesRelationshipKey =
      RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(),
                      &visitedStatement->getEntityKey(),
                      &modifiedEntityFromSource->getEntityKey());

  // we check uses first, as we don't want to skip case where assign statement
  // uses and modifies same variable that was modified by source statement
  // statement is affected by source statement
  if ((getRelationshipStorage()->getRelationship(usesRelationshipKey) !=
       nullptr) &&
      isValidEntityInput(visitedStatement)) {
    // only add if its an assign statement
    partialResults->push_back(visitedStatement);
  }

  if (isAllowedModifier(visitedStatement)) {
    auto modifiesRelationshipKey =
        RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(),
                        &visitedStatement->getEntityKey(),
                        &modifiedEntityFromSource->getEntityKey());

    // there is a modification, we terminate traversing this path
    if (getRelationshipStorage()->getRelationship(modifiesRelationshipKey) !=
        nullptr) {
      return true;
    }
  }

  return false;
}

auto AffectsCFGEvaluator::visitInReverseEvaluation(
    Statement* visitedStatement, std::vector<Statement*>* partialResults)
    -> bool {
  // we terminate traversing this path once we have encountered modifiers
  // for all variables used by source statement along this path
  if (usedEntitiesFromSource->empty()) {
    return true;
  }
  auto* modifiedVariables =
      getRelationshipStorage()
          ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
              ModifiesRelationship::getRelationshipTypeStatic(),
              visitedStatement->getEntityKey(),
              Variable::getEntityTypeStatic());

  // early return if statement does not modify any variables or is a container
  // stmt
  if (modifiedVariables == nullptr || modifiedVariables->empty() ||
      !isAllowedModifier(visitedStatement)) {
    return false;
  }

  // source statement is affected by this assign statement
  if (isValidEntityInput(visitedStatement) &&
      usedEntitiesFromSource->find(modifiedVariables->at(0)->getEntityKey()) !=
          usedEntitiesFromSource->end()) {
    usedEntitiesFromSource->erase(modifiedVariables->at(0)->getEntityKey());
    partialResults->push_back(visitedStatement);
    return false;
  }

  // we check if any of the variables used by source statement is modified by
  // the visited stmt and remove it from the map if it is
  if (modifiedVariables->size() < usedEntitiesFromSource->size()) {
    for (const auto& modifiedVariable : *modifiedVariables) {
      if (usedEntitiesFromSource->find(modifiedVariable->getEntityKey()) !=
          usedEntitiesFromSource->end()) {
        usedEntitiesFromSource->erase(modifiedVariable->getEntityKey());
      }
    }

  } else {
    std::vector<EntityKey*> usedVariablesToDelete;
    for (const auto& usedVariablePair : *usedEntitiesFromSource) {
      auto* usedVariable = usedVariablePair.second;
      auto relationshipKey = RelationshipKey(
          &ModifiesRelationship::getRelationshipTypeStatic(),
          &visitedStatement->getEntityKey(), &usedVariable->getEntityKey());

      if (getRelationshipStorage()->getRelationship(relationshipKey) !=
          nullptr) {
        usedVariablesToDelete.push_back(&usedVariable->getEntityKey());
      }
    }
    // we cannot delete from the map while iterating over it, so we store the
    // keys to delete and delete them after
    for (const auto& usedVariableToDelete : usedVariablesToDelete) {
      usedEntitiesFromSource->erase(*usedVariableToDelete);
    }
  }

  return false;
}
