//
// Created by Aiken Wong on 4/2/23.
//

#include "QueryFacade.h"

#include <utility>

#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

QueryFacade::QueryFacade(EntityManager *entityManager,
                         RelationshipManager *relationshipManager,
                         PatternManager *patternManager, CFGStorage *cfgManager)
    : entityManager(entityManager),
      relationshipManager(relationshipManager),
      cfgManager(cfgManager),
      patternManager(patternManager) {}

auto QueryFacade::getAllAssignStatements() -> std::vector<AssignStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<AssignStatement *> *>(
      this->entityManager->getEntitiesByType(
          AssignStatement::getEntityTypeStatic()));
}

auto QueryFacade::getAllIfStatements() -> std::vector<IfStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<IfStatement *> *>(
      this->entityManager->getEntitiesByType(
          IfStatement::getEntityTypeStatic()));
}

auto QueryFacade::getAllWhileStatements() -> std::vector<WhileStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<WhileStatement *> *>(
      this->entityManager->getEntitiesByType(
          WhileStatement::getEntityTypeStatic()));
}

auto QueryFacade::getAllCallStatements() -> std::vector<CallStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<CallStatement *> *>(
      this->entityManager->getEntitiesByType(
          CallStatement::getEntityTypeStatic()));
}

auto QueryFacade::getAllReadStatements() -> std::vector<ReadStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<ReadStatement *> *>(
      this->entityManager->getEntitiesByType(
          ReadStatement::getEntityTypeStatic()));
}

auto QueryFacade::getAllPrintStatements() -> std::vector<PrintStatement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<PrintStatement *> *>(
      this->entityManager->getEntitiesByType(
          PrintStatement::getEntityTypeStatic()));
}
auto QueryFacade::getAllProcedures() -> std::vector<Procedure *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<Procedure *> *>(
      this->entityManager->getEntitiesByType(Procedure::getEntityTypeStatic()));
}

auto QueryFacade::getAllVariables() -> std::vector<Variable *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<Variable *> *>(
      this->entityManager->getEntitiesByType(Variable::getEntityTypeStatic()));
}

auto QueryFacade::getAllConstants() -> std::vector<Constant *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<Constant *> *>(
      this->entityManager->getEntitiesByType(Constant::getEntityTypeStatic()));
}

auto QueryFacade::getAllStatements() -> std::vector<Statement *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<Statement *> *>(
      this->entityManager->getEntitiesByType(Statement::getEntityTypeStatic()));
}

auto QueryFacade::getParentRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<ParentRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<ParentRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          ParentRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getFollowsRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<FollowsRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<FollowsRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          FollowsRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getModifiesRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<ModifiesRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<ModifiesRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          ModifiesRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getWhileStatementsUsingVariableInCondition(
    std::string variableName) -> std::unordered_set<WhileStatement *> * {
  return this->patternManager->getWhileStatementsByConditionVariable(
      &variableName);
}

auto QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<UsesRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<UsesRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          UsesRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getIfStatementsUsingVariableInCondition(
    std::string variableName) -> std::unordered_set<IfStatement *> * {
  return this->patternManager->getIfStatementsByConditionVariable(
      &variableName);
}

auto QueryFacade::getVariablesInWhileStatementCondition(int statementNumber)
    -> std::unordered_set<Variable *> * {
  return this->patternManager->getVariablesUsedInWhileStatementCondition(
      statementNumber);
}

auto QueryFacade::getVariablesInIfStatementCondition(int statementNumber)
    -> std::unordered_set<Variable *> * {
  return this->patternManager->getVariablesUsedInIfStatementCondition(
      statementNumber);
}

auto QueryFacade::getEntity(const EntityType &entityType, int entityValue)
    -> Entity * {
  EntityKey entityKey = EntityKey(&entityType, entityValue);
  return this->entityManager->getEntity(entityKey);
}

auto QueryFacade::getEntity(const EntityType &entityType,
                            std::string entityValue) -> Entity * {
  EntityKey entityKey = EntityKey(&entityType, &entityValue);
  return this->entityManager->getEntity(entityKey);
}

auto QueryFacade::getEntitiesByType(const EntityType &entityType)
    -> std::vector<Entity *> * {
  return this->entityManager->getEntitiesByType(entityType);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  int leftEntityValue,
                                  const EntityType &rightEntityType,
                                  int rightEntityValue) -> Relationship * {
  auto *leftEntity = this->getEntity(leftEntityType, leftEntityValue);
  auto *rightEntity = this->getEntity(rightEntityType, rightEntityValue);

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto leftEntityKey = leftEntity->getEntityKey();
  auto rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  std::string leftEntityValue,
                                  const EntityType &rightEntityType,
                                  std::string rightEntityValue)
    -> Relationship * {
  auto *leftEntity =
      this->getEntity(leftEntityType, std::move(leftEntityValue));
  auto *rightEntity =
      this->getEntity(rightEntityType, std::move(rightEntityValue));

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto leftEntityKey = leftEntity->getEntityKey();
  auto rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  int leftEntityValue,
                                  const EntityType &rightEntityType,
                                  std::string rightEntityValue)
    -> Relationship * {
  auto *leftEntity = this->getEntity(leftEntityType, leftEntityValue);
  auto *rightEntity =
      this->getEntity(rightEntityType, std::move(rightEntityValue));

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto leftEntityKey = leftEntity->getEntityKey();
  auto rightEntityKey = rightEntity->getEntityKey();

  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationship(const RelationshipType &relationshipType,
                                  const EntityType &leftEntityType,
                                  std::string leftEntityValue,
                                  const EntityType &rightEntityType,
                                  int rightEntityValue) -> Relationship * {
  auto *leftEntity =
      this->getEntity(leftEntityType, std::move(leftEntityValue));
  auto *rightEntity = this->getEntity(rightEntityType, rightEntityValue);

  if (leftEntity == nullptr || rightEntity == nullptr) {
    return nullptr;
  }

  auto leftEntityKey = leftEntity->getEntityKey();
  auto rightEntityKey = rightEntity->getEntityKey();
  RelationshipKey relationshipKey =
      RelationshipKey(&relationshipType, &leftEntityKey, &rightEntityKey);

  return this->relationshipManager->getRelationship(relationshipKey, leftEntity, rightEntity);
}

auto QueryFacade::getRelationshipsByTypes(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType) -> std::vector<Relationship *> * {
  return this->relationshipManager->getRelationshipsByTypes(
      relationshipType, leftEntityType, rightEntityType);
}

auto QueryFacade::getRelationshipsByLeftEntityTypeAndRightEntityLiteral(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType, int rightEntityValue)
    -> std::vector<Entity *> * {
  EntityKey rightEntityKey = EntityKey(&rightEntityType, rightEntityValue);

  Entity *rightEntity = this->entityManager->getEntity(rightEntityKey);

  if (rightEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
          relationshipType, leftEntityType, rightEntity->getEntityKey());
}

auto QueryFacade::getRelationshipsByLeftEntityTypeAndRightEntityLiteral(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    const EntityType &rightEntityType, std::string rightEntityValue)
    -> std::vector<Entity *> * {
  EntityKey rightEntityKey = EntityKey(&rightEntityType, &rightEntityValue);

  Entity *rightEntity = this->entityManager->getEntity(rightEntityKey);

  if (rightEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
          relationshipType, leftEntityType, rightEntity->getEntityKey());
}

auto QueryFacade::getRelationshipsByLeftEntityLiteralAndRightEntityType(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    int leftEntityValue, const EntityType &rightEntityType)
    -> std::vector<Entity *> * {
  EntityKey leftEntityKey = EntityKey(&leftEntityType, leftEntityValue);

  Entity *leftEntity = this->entityManager->getEntity(leftEntityKey);

  if (leftEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
          relationshipType, leftEntity->getEntityKey(), rightEntityType);
}

auto QueryFacade::getRelationshipsByLeftEntityLiteralAndRightEntityType(
    const RelationshipType &relationshipType, const EntityType &leftEntityType,
    std::string leftEntityValue, const EntityType &rightEntityType)
    -> std::vector<Entity *> * {
  EntityKey leftEntityKey = EntityKey(&leftEntityType, &leftEntityValue);

  Entity *leftEntity = this->entityManager->getEntity(leftEntityKey);

  if (leftEntity == nullptr) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return this->relationshipManager
      ->getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
          relationshipType, leftEntity->getEntityKey(), rightEntityType);
}

void QueryFacade::clearCache() {
  // todo: clear cache
  this->relationshipManager->clearCache();
}
