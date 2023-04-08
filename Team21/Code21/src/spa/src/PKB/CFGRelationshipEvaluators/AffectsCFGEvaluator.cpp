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

AffectsCFGEvaluator::AffectsCFGEvaluator(cfg::CFG *cfg, RelationshipStorage *relationshipStorage,
                                         RelationshipCache *relationshipCache,
                                         EntityManager *entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, relationshipCache, entityManager),
      modifiedEntityFromSource(nullptr),
      currentUnusedVariables(nullptr) {}

auto AffectsCFGEvaluator::getRelationshipType() const
    -> const RelationshipType& {
  return AffectsRelationship::getRelationshipTypeStatic();
}

auto AffectsCFGEvaluator::getRelatedStatements(Statement* sourceStatement,
                                               bool isReverse)
    -> std::unique_ptr<std::vector<Entity*>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getRelationshipCache(),
                                    getEntityManager());

  isReverse ? initializeReverseEvaluation(sourceStatement)
            : initializeForwardsEvaluation(sourceStatement);

  auto results = std::make_unique<std::vector<Entity*>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();
  auto unusedVariablesForStatementsYetToVisit =
      std::stack<std::shared_ptr<std::unordered_map<std::string, Entity*>>>();

  // push all statements from results into stack

  auto* directRelations =
      nextCFGEvaluator
          .getCachedEntitiesAndRelationships(isReverse, *sourceStatement,
                                             Statement::getEntityTypeStatic())
          .first;

  if (directRelations == nullptr) {
    directRelations =
        nextCFGEvaluator
            .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                Statement::getEntityTypeStatic(), sourceStatement, isReverse)
            .first;
  }

  for (auto* result : *directRelations) {
    statementNumbersToVisit.push(*result->getEntityKey().getOptionalInt());
    if (isReverse) {
      unusedVariablesForStatementsYetToVisit.push(this->currentUnusedVariables);
    }
  }

  while (!statementNumbersToVisit.empty()) {
    auto nextToVisit = statementNumbersToVisit.top();
    statementNumbersToVisit.pop();
    if (isReverse) {
      currentUnusedVariables = unusedVariablesForStatementsYetToVisit.top();
      unusedVariablesForStatementsYetToVisit.pop();
    }
    if (visitedStatementNumbers[nextToVisit]) {
      continue;
    }

    visitedStatementNumbers[nextToVisit] = true;

    auto* nextToVisitStmt = getEntityManager()->getStmtByNumber(nextToVisit);

    if (isReverse
            ? visitInReverseEvaluation(
                  nextToVisitStmt,
                  reinterpret_cast<std::vector<Statement*>*>(results.get()))
            : visitInForwardsEvaluation(
                  nextToVisitStmt,
                  reinterpret_cast<std::vector<Statement*>*>(results.get()))) {
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

    for (auto * neighbour : *neighbours) {
      statementNumbersToVisit.push(*neighbour->getEntityKey().getOptionalInt());
      if (isReverse) {
        unusedVariablesForStatementsYetToVisit.push(
            this->currentUnusedVariables);
      }
    }
  }

  return results;
}

auto AffectsCFGEvaluator::createNewRelationship(Entity* leftStatement,
                                                Entity* rightStatement)
    -> std::unique_ptr<Relationship> {
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
  return std::make_unique<AffectsRelationship>(leftAssignStatement,
                                               rightAssignStatement);
}

void AffectsCFGEvaluator::initializeForwardsEvaluation(
    Statement* sourceStatement) {
  // assign statements only modify one variable, so we can just get the first
  // one
  modifiedEntityFromSource =
      getRelationshipStorage()
          ->getEntitiesFromLiteralSynonymStore(
              ModifiesRelationship::getRelationshipTypeStatic(),
              sourceStatement->getEntityKey(), Variable::getEntityTypeStatic())
          ->at(0);
}

void AffectsCFGEvaluator::initializeReverseEvaluation(
    Statement* sourceStatement) {
  auto* usedEntitiesFromSourceVector =
      getRelationshipStorage()->getEntitiesFromLiteralSynonymStore(
          UsesRelationship::getRelationshipTypeStatic(),
          sourceStatement->getEntityKey(), Variable::getEntityTypeStatic());

  currentUnusedVariables =
      std::make_shared<std::unordered_map<std::string, Entity*>>();

  if (usedEntitiesFromSourceVector == nullptr) {
    return;
  }

  for (const auto& usedEntity : *usedEntitiesFromSourceVector) {
    currentUnusedVariables->insert({*usedEntity->getEntityValue(), usedEntity});
  }
}

auto AffectsCFGEvaluator::visitInForwardsEvaluation(
    Statement* visitedStatement, std::vector<Statement*>* partialResults)
    -> bool {
  // we check uses first, as we don't want to skip case where assign statement
  // uses and modifies same variable that was modified by source statement
  // statement is affected by source statement
  if (isValidEntityInput(visitedStatement)) {
    auto usesRelationshipKey =
        RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(),
                        &visitedStatement->getEntityKey(),
                        &modifiedEntityFromSource->getEntityKey());
    if (getRelationshipStorage()->getRelationshipFromStore(
            usesRelationshipKey) != nullptr) {
      partialResults->push_back(visitedStatement);
    }
  }

  if (isAllowedModifier(visitedStatement)) {
    auto modifiesRelationshipKey =
        RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(),
                        &visitedStatement->getEntityKey(),
                        &modifiedEntityFromSource->getEntityKey());

    // there is a modification, we terminate traversing this path
    if (getRelationshipStorage()->getRelationshipFromStore(
            modifiesRelationshipKey) != nullptr) {
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
  if (currentUnusedVariables->empty()) {
    return true;
  }

  if (!isAllowedModifier(visitedStatement)) {
    return false;
  }
  auto* modifiedVariables =
      getRelationshipStorage()->getEntitiesFromLiteralSynonymStore(
          ModifiesRelationship::getRelationshipTypeStatic(),
          visitedStatement->getEntityKey(), Variable::getEntityTypeStatic());

  // early return if statement does not modify any variables or is a container
  // stmt
  if (modifiedVariables == nullptr || modifiedVariables->empty()) {
    return false;
  }

  // source statement is affected by this assign statement
  if (isValidEntityInput(visitedStatement) &&
      currentUnusedVariables->find(
          *modifiedVariables->at(0)->getEntityValue()) !=
          currentUnusedVariables->end()) {
    // copy before making modifications
    currentUnusedVariables =
        std::make_shared<std::unordered_map<std::string, Entity*>>(
            *currentUnusedVariables);
    currentUnusedVariables->erase(*modifiedVariables->at(0)->getEntityValue());
    partialResults->push_back(visitedStatement);
    return false;
  }

  // we check if any of the variables used by source statement is modified by
  // the visited stmt and remove it from the map if it is
  if (modifiedVariables->size() < currentUnusedVariables->size()) {
    // copy before making modifications

    currentUnusedVariables =
        std::make_shared<std::unordered_map<std::string, Entity*>>(
            *currentUnusedVariables);

    for (const auto& modifiedVariable : *modifiedVariables) {
      if (currentUnusedVariables->find(*modifiedVariable->getEntityValue()) !=
          currentUnusedVariables->end()) {
        currentUnusedVariables->erase(*modifiedVariable->getEntityValue());
      }
    }

  } else {
    std::vector<std::string*> usedVariablesToDelete;
    for (const auto& usedVariablePair : *currentUnusedVariables) {
      auto* usedVariable = usedVariablePair.second;
      auto relationshipKey = RelationshipKey(
          &ModifiesRelationship::getRelationshipTypeStatic(),
          &visitedStatement->getEntityKey(), &usedVariable->getEntityKey());

      if (getRelationshipStorage()->getRelationshipFromStore(relationshipKey) !=
          nullptr) {
        usedVariablesToDelete.push_back(usedVariable->getEntityValue());
      }
    }

    if (!usedVariablesToDelete.empty()) {
      // copy before making modifications
      currentUnusedVariables =
          std::make_shared<std::unordered_map<std::string, Entity*>>(
              *currentUnusedVariables);
    }
    // we cannot delete from the map while iterating over it, so we store the
    // keys to delete and delete them after
    for (const auto& usedVariableToDelete : usedVariablesToDelete) {
      // copy before making modifications
      currentUnusedVariables =
          std::make_shared<std::unordered_map<std::string, Entity*>>(
              *currentUnusedVariables);
      currentUnusedVariables->erase(*usedVariableToDelete);
    }
  }

  return false;
}
