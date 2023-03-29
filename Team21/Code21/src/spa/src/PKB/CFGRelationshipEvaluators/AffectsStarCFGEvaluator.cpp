//
// Created by Aiken Wong on 29/3/23.
//

#include "AffectsStarCFGEvaluator.h"

#include <stack>
#include <stdexcept>

#include "NextCFGEvaluator.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

AffectsStarCFGEvaluator::AffectsStarCFGEvaluator(cfg::CFG* cfg,
                                                 RelationshipStorage* relationshipStorage,
                                                 EntityManager* entityManager)
    : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsStarCFGEvaluator::getRelationshipType() const -> const RelationshipType& {
  return AffectsStarRelationship::getRelationshipTypeStatic();
}

auto AffectsStarCFGEvaluator::getRelatedBlockStatementPairs(
    std::pair<cfg::Block*, Statement*>& sourceBlockStatementPair,
    bool isReverse)
    -> std::shared_ptr<
        std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>> {
  auto statement = sourceBlockStatementPair.second;
  if (!isValidEntityInput(sourceBlockStatementPair.second)) {
    throw std::invalid_argument(
        "AffectsCFGEvaluator::getRelatedBlockStatementPairs: "
        "sourceBlockStatementPair.second is not a valid entity");
  }

  auto visitedStatementNumbers = std::unordered_set<int>();

  NextCFGEvaluator nextCFGEvaluator(cfg, relationshipStorage, entityManager);

  auto initialModifiedVariable = relationshipStorage->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                                                        ModifiesRelationship::getRelationshipTypeStatic(),
                                                        statement->getEntityKey(),
                                                        Variable::getEntityTypeStatic())
                                     ->at(0);

  auto results = std::make_shared<
      std::vector<std::shared_ptr<std::pair<cfg::Block*, Statement*>>>>();

  // create stack of statements to visit
  auto statementsToVisit =
      std::stack<std::shared_ptr<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>>();

  // push all statements from results into stack

  for (auto result : *nextCFGEvaluator.getRelatedBlockStatementPairs(
           sourceBlockStatementPair, isReverse)) {
    statementsToVisit.push(std::make_shared<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>(result, initialModifiedVariable));
  }

  // bfs
  while (!statementsToVisit.empty()) {
    auto nextToVisit = statementsToVisit.top();
    statementsToVisit.pop();

    auto nextPair = nextToVisit->first;
    auto statement = nextToVisit->first->second;

    auto modifiedVariable = nextToVisit->second;
    if (visitedStatementNumbers.find(
            statement->getStatementNumber()) !=
        visitedStatementNumbers.end()) {
      continue;
    }

    visitedStatementNumbers.insert(statement->getStatementNumber());

    auto usesRelationshipKey = RelationshipKey(
        &UsesRelationship::getRelationshipTypeStatic(),
        &statement->getEntityKey(),
        &modifiedVariable->getEntityKey());

    auto modifiesRelationshipKey = RelationshipKey(
        &ModifiesRelationship::getRelationshipTypeStatic(),
        &statement->getEntityKey(),
        &modifiedVariable->getEntityKey());

    // we check uses first, as we don't want to skip case where assign statement uses and modifies same variable that was modified by source statement
    // statement is affected by source statement
    if (relationshipStorage->getRelationship(usesRelationshipKey) && isValidEntityInput(statement)) {
      // only add if its an assign statement
      results->push_back(nextPair);
      // get new modified variable
      modifiedVariable = relationshipStorage->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
                                                ModifiesRelationship::getRelationshipTypeStatic(),
                                                statement->getEntityKey(),
                                                Variable::getEntityTypeStatic())
                             ->at(0);

    } else if (relationshipStorage->getRelationship(modifiesRelationshipKey)) {
      // there is a modification by non assign statement, we terminate traversing this path
      continue;
    }

    auto nextResults =
        nextCFGEvaluator.getRelatedBlockStatementPairs(*nextPair, isReverse);

    for (auto nextResult : *nextResults) {
      statementsToVisit.push(std::make_shared<std::pair<std::shared_ptr<std::pair<cfg::Block*, Statement*>>, Entity*>>(nextResult, modifiedVariable));
    }
  }

  return results;
}

auto AffectsStarCFGEvaluator::createNewRelationship(Statement* leftStatement,
                                                    Statement* rightStatement)
    -> std::shared_ptr<Relationship> {
  auto leftAssignStatement = dynamic_cast<AssignStatement*>(leftStatement);
  auto rightAssignStatement = dynamic_cast<AssignStatement*>(rightStatement);

  if (leftAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsStarCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  } else if (rightAssignStatement == nullptr) {
    throw std::runtime_error(
        "AffectsStarCFGEvaluator::createNewRelationship: "
        "leftStatement or rightStatement is not an "
        "AssignStatement");
  }
  return std::make_shared<AffectsStarRelationship>(leftAssignStatement, rightAssignStatement);
}

auto AffectsStarCFGEvaluator::isValidEntityInput(Entity* entity) -> bool {
  return entity->getEntityType() == AssignStatement::getEntityTypeStatic();
}

auto AffectsStarCFGEvaluator::isValidEntityTypeInput(const EntityType& entityType) -> bool {
  entityManager->getEntitiesByType(entityType);
  return entityType == AssignStatement::getEntityTypeStatic() ||
         entityType == Statement::getEntityTypeStatic();
}