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

auto QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<UsesRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<UsesRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          UsesRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getParentStarRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<ParentStarRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<ParentStarRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          ParentStarRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getFollowsStarRelationshipsByLeftAndRightEntityTypes(
    const EntityType &leftEntityType, const EntityType &rightEntityType)
    -> std::vector<FollowsStarRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<FollowsStarRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          FollowsStarRelationship::getRelationshipTypeStatic(), leftEntityType,
          rightEntityType));
}

auto QueryFacade::getAllCallsRelationships()
    -> std::vector<CallsRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<CallsRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          CallsRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic()));
}

auto QueryFacade::getAllCallsStarRelationships()
    -> std::vector<CallsStarRelationship *> * {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<std::vector<CallsStarRelationship *> *>(
      this->relationshipManager->getRelationshipsByTypes(

          CallsStarRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic()));
}

auto QueryFacade::getStatementModifiesVariableRelationship(
    int statementNumber, std::string variableName) -> ModifiesRelationship * {
  EntityKey statementKey =
      EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
  EntityKey variableKey =
      EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *statement =
      dynamic_cast<Statement *>(this->entityManager->getEntity(statementKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  if (statement == nullptr || variable == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(),
                      &statementKey, &variableKey);

  return dynamic_cast<ModifiesRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getProcedureModifiesVariableRelationship(
    std::string procedureName, std::string variableName)
    -> ModifiesRelationship * {
  EntityKey procedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
  EntityKey variableKey =
      EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *procedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(procedureKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  if (procedure == nullptr || variable == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(),
                      &procedureKey, &variableKey);

  return dynamic_cast<ModifiesRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getStatementUsesVariableRelationship(int statementNumber,
                                                       std::string variableName)
    -> UsesRelationship * {
  EntityKey statementKey =
      EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
  EntityKey variableKey =
      EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *statement =
      dynamic_cast<Statement *>(this->entityManager->getEntity(statementKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  if (statement == nullptr || variable == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(),
                      &statementKey, &variableKey);

  return dynamic_cast<UsesRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getProcedureUsesVariableRelationship(
    std::string procedureName, std::string variableName) -> UsesRelationship * {
  EntityKey procedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
  EntityKey variableKey =
      EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *procedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(procedureKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  if (procedure == nullptr || variable == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(),
                      &procedureKey, &variableKey);

  return dynamic_cast<UsesRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}
auto QueryFacade::getParentRelationship(int parentStatementNumber,
                                        int childStatementNumber)
    -> ParentRelationship * {
  EntityKey parentStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
  EntityKey childStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);

  auto *parentStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(parentStatementKey));
  auto *childStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(childStatementKey));

  if (parentStatement == nullptr || childStatement == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&ParentRelationship::getRelationshipTypeStatic(),
                      &parentStatementKey, &childStatementKey);

  return dynamic_cast<ParentRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getFollowsRelationship(int firstStatementNumber,
                                         int secondStatementNumber)
    -> FollowsRelationship * {
  EntityKey firstStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
  EntityKey secondStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);

  auto *firstStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(firstStatementKey));
  auto *secondStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(secondStatementKey));

  if (firstStatement == nullptr || secondStatement == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&FollowsRelationship::getRelationshipTypeStatic(),
                      &firstStatementKey, &secondStatementKey);

  return dynamic_cast<FollowsRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getParentStarRelationship(int parentStatementNumber,
                                            int childStatementNumber)
    -> ParentStarRelationship * {
  EntityKey parentStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
  EntityKey childStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);

  auto *parentStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(parentStatementKey));
  auto *childStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(childStatementKey));

  if (parentStatement == nullptr || childStatement == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&ParentStarRelationship::getRelationshipTypeStatic(),
                      &parentStatementKey, &childStatementKey);

  return dynamic_cast<ParentStarRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getFollowsStarRelationship(int firstStatementNumber,
                                             int secondStatementNumber)
    -> FollowsStarRelationship * {
  EntityKey firstStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
  EntityKey secondStatementKey =
      EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);

  auto *firstStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(firstStatementKey));
  auto *secondStatement = dynamic_cast<Statement *>(
      this->entityManager->getEntity(secondStatementKey));

  if (firstStatement == nullptr || secondStatement == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&FollowsStarRelationship::getRelationshipTypeStatic(),
                      &firstStatementKey, &secondStatementKey);

  return dynamic_cast<FollowsStarRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getCallsRelationship(std::string callerName,
                                       std::string calleeName)
    -> CallsRelationship * {
  EntityKey callerKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &callerName);
  EntityKey calleeKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &calleeName);

  auto *callerProcedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(callerKey));
  auto *calleeProcedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(calleeKey));

  if (callerProcedure == nullptr || calleeProcedure == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey = RelationshipKey(
      &CallsRelationship::getRelationshipTypeStatic(), &callerKey, &calleeKey);

  return dynamic_cast<CallsRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getCallsStarRelationship(std::string callerName,
                                           std::string calleeName)
    -> CallsStarRelationship * {
  EntityKey callerKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &callerName);
  EntityKey calleeKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &calleeName);

  auto *callerProcedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(callerKey));
  auto *calleeProcedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(calleeKey));

  if (callerProcedure == nullptr || calleeProcedure == nullptr) {
    return nullptr;
  }

  RelationshipKey relationshipKey =
      RelationshipKey(&CallsStarRelationship::getRelationshipTypeStatic(),
                      &callerKey, &calleeKey);

  return dynamic_cast<CallsStarRelationship *>(
      this->relationshipManager->getRelationship(relationshipKey));
}

auto QueryFacade::getCFG() -> cfg::CFG * { return this->cfgManager->getCFG(); }

auto QueryFacade::getWhileStatementsUsingVariableInCondition(
    std::string variableName) -> std::unordered_set<WhileStatement *> * {
  return this->patternManager->getWhileStatementsByConditionVariable(
      &variableName);
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

  return this->relationshipManager->getRelationship(relationshipKey);
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

  return this->relationshipManager->getRelationship(relationshipKey);
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

  return this->relationshipManager->getRelationship(relationshipKey);
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

  return this->relationshipManager->getRelationship(relationshipKey);
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
