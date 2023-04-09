//
// Created by Aiken Wong on 4/2/23.
//

#include "PopulateFacade.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

PopulateFacade::PopulateFacade(EntityManager *entityManager,
                               RelationshipManager *relationshipManager,
                               PatternManager *patternManager,
                               CFGStorage *cfgManager)
    : entityManager(entityManager),
      relationshipManager(relationshipManager),
      patternManager(patternManager),
      cfgManager(cfgManager) {}

void PopulateFacade::storeAssignmentStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<AssignStatement>(statementNumber));
}

void PopulateFacade::storeCallStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<CallStatement>(statementNumber));
}

void PopulateFacade::storeIfStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<IfStatement>(statementNumber));
}

void PopulateFacade::storePrintStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<PrintStatement>(statementNumber));
}

void PopulateFacade::storeReadStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<ReadStatement>(statementNumber));
}

void PopulateFacade::storeWhileStatement(int statementNumber) {
  this->entityManager->storeEntity(
      std::make_unique<WhileStatement>(statementNumber));
}

void PopulateFacade::storeConstant(int constantValue) {
  this->entityManager->storeEntity(std::make_unique<Constant>(constantValue));
}

void PopulateFacade::storeVariable(const std::string &variableName) {
  this->entityManager->storeEntity(
      std::make_unique<Variable>(std::make_unique<std::string>(variableName)));
}

void PopulateFacade::storeProcedure(const std::string &procedureName) {
  this->entityManager->storeEntity(std::make_unique<Procedure>(
      std::make_unique<std::string>(procedureName)));
}

void PopulateFacade::storeStatementModifiesVariableRelationship(
    int statementNumber, std::string variableName) {
  auto statementKey =
      EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
  auto *statement =
      dynamic_cast<Statement *>(this->entityManager->getEntity(statementKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));
  PopulateFacade::validateEntityExists(statement);
  PopulateFacade::validateEntityExists(variable);
  this->relationshipManager->storeRelationship(
      std::make_unique<ModifiesRelationship>(statement, variable));
}

void PopulateFacade::storeStatementUsesVariableRelationship(
    int statementNumber, std::string variableName) {
  auto statementKey =
      EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
  auto *statement =
      dynamic_cast<Statement *>(this->entityManager->getEntity(statementKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));
  PopulateFacade::validateEntityExists(statement);
  PopulateFacade::validateEntityExists(variable);
  this->relationshipManager->storeRelationship(
      std::make_unique<UsesRelationship>(statement, variable));
}

void PopulateFacade::storeFollowsRelationship(int firstStatementNumber,
                                              int secondStatementNumber) {
  auto *firstStatement =
      this->entityManager->getStmtByNumber(firstStatementNumber);
  auto *secondStatement =
      this->entityManager->getStmtByNumber(secondStatementNumber);
  PopulateFacade::validateEntityExists(firstStatement);
  PopulateFacade::validateEntityExists(secondStatement);

  this->relationshipManager->storeRelationship(
      std::make_unique<FollowsRelationship>(firstStatement, secondStatement));
}

void PopulateFacade::storeParentRelationship(int parentStatementNumber,
                                             int childStatementNumber) {
  auto *parentStatement =
      this->entityManager->getStmtByNumber(parentStatementNumber);
  auto *childStatement =
      this->entityManager->getStmtByNumber(childStatementNumber);

  PopulateFacade::validateEntityExists(parentStatement);
  PopulateFacade::validateEntityExists(childStatement);
  this->relationshipManager->storeRelationship(
      std::make_unique<ParentRelationship>(parentStatement, childStatement));
}

void PopulateFacade::storeProcedureModifiesVariableRelationship(
    std::string procedureName, std::string variableName) {
  auto procedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
  auto *procedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(procedureKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));
  PopulateFacade::validateEntityExists(procedure);
  PopulateFacade::validateEntityExists(variable);
  this->relationshipManager->storeRelationship(
      std::make_unique<ModifiesRelationship>(procedure, variable));
}

void PopulateFacade::storeProcedureUsesVariableRelationship(
    std::string procedureName, std::string variableName) {
  auto procedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
  auto *procedure =
      dynamic_cast<Procedure *>(this->entityManager->getEntity(procedureKey));
  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));
  PopulateFacade::validateEntityExists(procedure);
  PopulateFacade::validateEntityExists(variable);
  this->relationshipManager->storeRelationship(
      std::make_unique<UsesRelationship>(procedure, variable));
}

void PopulateFacade::storeParentStarRelationship(int parentStatementNumber,
                                                 int childStatementNumber) {
  auto *parentStatement =
      this->entityManager->getStmtByNumber(parentStatementNumber);
  auto *childStatement =
      this->entityManager->getStmtByNumber(childStatementNumber);
  PopulateFacade::validateEntityExists(parentStatement);
  PopulateFacade::validateEntityExists(childStatement);
  this->relationshipManager->storeRelationship(
      std::make_unique<ParentStarRelationship>(parentStatement,
                                               childStatement));
}

void PopulateFacade::storeFollowsStarRelationship(int firstStatementNumber,
                                                  int secondStatementNumber) {
  auto *firstStatement =
      this->entityManager->getStmtByNumber(firstStatementNumber);
  auto *secondStatement =
      this->entityManager->getStmtByNumber(secondStatementNumber);
  PopulateFacade::validateEntityExists(firstStatement);
  PopulateFacade::validateEntityExists(secondStatement);
  this->relationshipManager->storeRelationship(
      std::make_unique<FollowsStarRelationship>(firstStatement,
                                                secondStatement));
}

void PopulateFacade::storeAssignStatementPostfixExpression(
    int statementNumber, const std::string &postfixExpression) {
  auto assignStatementKey =
      EntityKey(&AssignStatement::getEntityTypeStatic(), statementNumber);
  auto *statement = dynamic_cast<AssignStatement *>(
      this->entityManager->getEntity(assignStatementKey));

  PopulateFacade::validateEntityExists(statement);
  PatternManager::storeAssignStatementPostfixExpression(
      statement, std::make_unique<std::string>(postfixExpression));
}

void PopulateFacade::storeCallStatementProcedureName(
    int statementNumber, const std::string &procedureName) {
  auto callStatementKey =
      EntityKey(&CallStatement::getEntityTypeStatic(), statementNumber);

  auto *callStatement = dynamic_cast<CallStatement *>(
      this->entityManager->getEntity(callStatementKey));
  PopulateFacade::validateEntityExists(callStatement);
  callStatement->setProcedureName(std::make_unique<std::string>(procedureName));
}

void PopulateFacade::storeIfStatementConditionVariable(
    int statementNumber, std::string variableName) {
  auto ifStatementKey =
      EntityKey(&IfStatement::getEntityTypeStatic(), statementNumber);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *statement = dynamic_cast<IfStatement *>(
      this->entityManager->getEntity(ifStatementKey));

  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  PopulateFacade::validateEntityExists(statement);
  PopulateFacade::validateEntityExists(variable);

  this->patternManager->storeIfStatementConditionVariable(statement, variable);
}

void PopulateFacade::storeWhileStatementConditionVariable(
    int statementNumber, std::string variableName) {
  auto whileStatementKey =
      EntityKey(&WhileStatement::getEntityTypeStatic(), statementNumber);
  auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

  auto *statement = dynamic_cast<WhileStatement *>(
      this->entityManager->getEntity(whileStatementKey));

  auto *variable =
      dynamic_cast<Variable *>(this->entityManager->getEntity(variableKey));

  PopulateFacade::validateEntityExists(statement);
  PopulateFacade::validateEntityExists(variable);

  this->patternManager->storeWhileStatementConditionVariable(statement,
                                                             variable);
}

void PopulateFacade::storeCallsRelationship(std::string caller,
                                            std::string callee) {
  auto firstProcedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &caller);
  auto secondProcedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &callee);
  auto *firstProcedure = dynamic_cast<Procedure *>(
      this->entityManager->getEntity(firstProcedureKey));
  auto *secondProcedure = dynamic_cast<Procedure *>(
      this->entityManager->getEntity(secondProcedureKey));
  PopulateFacade::validateEntityExists(firstProcedure);
  PopulateFacade::validateEntityExists(secondProcedure);
  this->relationshipManager->storeRelationship(
      std::make_unique<CallsRelationship>(firstProcedure, secondProcedure));
}

void PopulateFacade::storeCallsStarRelationship(std::string caller,
                                                std::string callee) {
  auto firstProcedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &caller);
  auto secondProcedureKey =
      EntityKey(&Procedure::getEntityTypeStatic(), &callee);
  auto *firstProcedure = dynamic_cast<Procedure *>(
      this->entityManager->getEntity(firstProcedureKey));
  auto *secondProcedure = dynamic_cast<Procedure *>(
      this->entityManager->getEntity(secondProcedureKey));
  PopulateFacade::validateEntityExists(firstProcedure);
  PopulateFacade::validateEntityExists(secondProcedure);
  this->relationshipManager->storeRelationship(
      std::make_unique<CallsStarRelationship>(firstProcedure, secondProcedure));
}

void PopulateFacade::validateEntityExists(Entity *entity) {
  if (entity == nullptr) {
    throw std::runtime_error(
        "Entity does not exist in PKB and the relationship cannot be added, "
        "please populate PKB with all entities before storing relationships.");
  }
}

void PopulateFacade::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
  this->relationshipManager->storeCFG(cfg);
  this->cfgManager->storeCFG(cfg);
}
