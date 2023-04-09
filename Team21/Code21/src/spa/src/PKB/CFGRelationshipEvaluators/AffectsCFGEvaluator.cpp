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

AffectsCFGEvaluator::AffectsCFGEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage,
                                         RelationshipCache* relationshipCache,
                                         EntityManager* entityManager)
    : AffectsRelatedCFGEvaluator(cfg, relationshipStorage, relationshipCache, entityManager),
      modifiedEntityFromSource(nullptr) {}

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

  if (isReverse) {
    return getRelatedStatementsInReverseEvaluation(sourceStatement);
  }
  return getRelatedStatementsInForwardsEvaluation(sourceStatement);
}

auto AffectsCFGEvaluator::createNewRelationship(Entity* leftStatement,
                                                Entity* rightStatement)
    -> std::unique_ptr<Relationship> {
  auto* leftAssignStatement = dynamic_cast<AssignStatement*>(leftStatement);
  auto* rightAssignStatement = dynamic_cast<AssignStatement*>(rightStatement);

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

auto AffectsCFGEvaluator::visitInForwardsEvaluation(
    Entity* visitedStatement, std::vector<Entity*>* partialResults)
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

auto AffectsCFGEvaluator::getRelatedStatementsInForwardsEvaluation(Statement* sourceStatement)
    -> std::unique_ptr<std::vector<Entity*>> {
  auto visitedStatementNumbers =
      std::vector<bool>(getEntityManager()->getNumberOfStatements() + 1, false);

  NextCFGEvaluator nextCFGEvaluator(getCFG(), getRelationshipStorage(), getRelationshipCache(),
                                    getEntityManager());

  initializeForwardsEvaluation(sourceStatement);

  auto results = std::make_unique<std::vector<Entity*>>();

  // create stack of statements to visit
  auto statementNumbersToVisit = std::stack<int>();
  auto unusedVariablesForStatementsYetToVisit =
      std::stack<std::shared_ptr<std::unordered_map<std::string, Entity*>>>();

  // push all statements from results into stack

  auto* directRelations =
      nextCFGEvaluator
          .getCachedEntitiesAndRelationships(false, *sourceStatement,
                                             Statement::getEntityTypeStatic())
          .cachedRelatedEntities;

  if (directRelations == nullptr) {
    directRelations =
        nextCFGEvaluator
            .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                Statement::getEntityTypeStatic(), sourceStatement, false)
            .cachedRelatedEntities;
  }

  for (auto* result : *directRelations) {
    statementNumbersToVisit.push(*result->getEntityKey().getOptionalInt());
  }

  while (!statementNumbersToVisit.empty()) {
    auto nextToVisit = statementNumbersToVisit.top();
    statementNumbersToVisit.pop();

    if (visitedStatementNumbers[nextToVisit]) {
      continue;
    }

    visitedStatementNumbers[nextToVisit] = true;

    auto* nextToVisitStmt = getEntityManager()->getStatementByNumber(nextToVisit);

    if (visitInForwardsEvaluation(
            nextToVisitStmt,
            results.get())) {
      continue;
    }

    auto* neighbours =
        nextCFGEvaluator
            .getCachedEntitiesAndRelationships(false, *nextToVisitStmt,
                                               Statement::getEntityTypeStatic())
            .cachedRelatedEntities;

    if (neighbours == nullptr) {
      neighbours =
          nextCFGEvaluator
              .evaluateAndCacheRelationshipsByGivenEntityTypeAndEntity(
                  Statement::getEntityTypeStatic(), nextToVisitStmt, false)
              .cachedRelatedEntities;
    }

    for (auto* neighbour : *neighbours) {
      statementNumbersToVisit.push(*neighbour->getEntityKey().getOptionalInt());
    }
  }

  return results;
}

// this is a naive algorithm, by completing all forwards evaluation first, then working backwards
auto AffectsCFGEvaluator::getRelatedStatementsInReverseEvaluation(Statement* sourceStatement)
    -> std::unique_ptr<std::vector<Entity*>> {
  auto* allAssignStatements = getEntityManager()->getEntitiesByType(AssignStatement::getEntityTypeStatic());

  auto results = std::make_unique<std::vector<Entity*>>();

  for (auto* assignStatement : *allAssignStatements) {
    RelationshipKey relationshipKey(&AffectsRelationship::getRelationshipTypeStatic(), &assignStatement->getEntityKey(),
                                    &sourceStatement->getEntityKey());

    auto* affectsRelationship = getRelationshipCache()->getCachedRelationship(
        relationshipKey);
    if (affectsRelationship == nullptr) {
      affectsRelationship = evaluateAndCacheRelationshipsByGivenEntities(
          assignStatement, sourceStatement);
    }

    if (affectsRelationship != nullptr) {
      results->push_back(assignStatement);
    }
  }

  return results;
}
