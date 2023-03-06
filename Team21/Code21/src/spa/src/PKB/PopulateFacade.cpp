//
// Created by Aiken Wong on 4/2/23.
//

#include "PopulateFacade.h"

#include <memory>
#include <stdexcept>

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

PopulateFacade::PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager, PatternManager *patternManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
    this->patternManager = patternManager;
}

void PopulateFacade::storeAssignmentStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<AssignStatement>(statementNumber));
}

void PopulateFacade::storeCallStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<CallStatement>(statementNumber));
}

void PopulateFacade::storeIfStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<IfStatement>(statementNumber));
}

void PopulateFacade::storePrintStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<PrintStatement>(statementNumber));
}

void PopulateFacade::storeReadStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<ReadStatement>(statementNumber));
}

void PopulateFacade::storeWhileStatement(int statementNumber) {
    this->entityManager->storeStatement(std::make_shared<WhileStatement>(statementNumber));
}

void PopulateFacade::storeConstant(int constantValue) {
    this->entityManager->storeConstant(std::make_shared<Constant>(constantValue));
}

void PopulateFacade::storeVariable(std::string variableName) {
    this->entityManager->storeVariable(std::make_shared<Variable>(new std::string(variableName)));
}

void PopulateFacade::storeProcedure(std::string procedureName) {
    this->entityManager->storeProcedure(std::make_shared<Procedure>(new std::string(procedureName)));
}

void PopulateFacade::storeStatementModifiesVariableRelationship(int statementNumber, std::string variableName) {
    Statement *statement = this->entityManager->getStatementByStatementNumber(statementNumber);
    Variable *variable = this->entityManager->getVariableByVariableName(variableName);
    this->validateEntityExists(statement);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(std::make_shared<ModifiesRelationship>(statement, variable));
}

void PopulateFacade::storeStatementUsesVariableRelationship(int statementNumber, std::string variableName) {
    Statement *statement = this->entityManager->getStatementByStatementNumber(statementNumber);
    Variable *variable = this->entityManager->getVariableByVariableName(variableName);
    this->validateEntityExists(statement);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(std::make_shared<UsesRelationship>(statement, variable));
}

void PopulateFacade::storeFollowsRelationship(int firstStatementNumber, int secondStatementNumber) {
    Statement *firstStatement = this->entityManager->getStatementByStatementNumber(firstStatementNumber);
    Statement *secondStatement = this->entityManager->getStatementByStatementNumber(secondStatementNumber);
    this->validateEntityExists(firstStatement);
    this->validateEntityExists(secondStatement);
    this->relationshipManager->storeRelationship(std::make_shared<FollowsRelationship>(firstStatement, secondStatement));
}

void PopulateFacade::storeParentRelationship(int parentStatementNumber, int childStatementNumber) {
    Statement *parentStatement = this->entityManager->getStatementByStatementNumber(parentStatementNumber);
    Statement *childStatement = this->entityManager->getStatementByStatementNumber(childStatementNumber);
    this->validateEntityExists(parentStatement);
    this->validateEntityExists(childStatement);
    this->relationshipManager->storeRelationship(std::make_shared<ParentRelationship>(parentStatement, childStatement));
}

void PopulateFacade::storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = this->entityManager->getProcedureByProcedureName(procedureName);
    Variable *variable = this->entityManager->getVariableByVariableName(variableName);
    this->validateEntityExists(procedure);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(std::make_shared<ModifiesRelationship>(procedure, variable));
}

void PopulateFacade::storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = this->entityManager->getProcedureByProcedureName(procedureName);
    Variable *variable = this->entityManager->getVariableByVariableName(variableName);
    this->validateEntityExists(procedure);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(std::make_shared<UsesRelationship>(procedure, variable));
}

void PopulateFacade::storeParentStarRelationship(int parentStatementNumber, int childStatementNumber) {
    Statement *parentStatement = this->entityManager->getStatementByStatementNumber(parentStatementNumber);
    Statement *childStatement = this->entityManager->getStatementByStatementNumber(childStatementNumber);
    this->validateEntityExists(parentStatement);
    this->validateEntityExists(childStatement);
    this->relationshipManager->storeRelationship(std::make_shared<ParentStarRelationship>(parentStatement, childStatement));
}

void PopulateFacade::storeFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber) {
    Statement *firstStatement = this->entityManager->getStatementByStatementNumber(firstStatementNumber);
    Statement *secondStatement = this->entityManager->getStatementByStatementNumber(secondStatementNumber);
    this->validateEntityExists(firstStatement);
    this->validateEntityExists(secondStatement);
    this->relationshipManager->storeRelationship(std::make_shared<FollowsStarRelationship>(firstStatement, secondStatement));
}

void PopulateFacade::storeAssignStatementPostfixExpression(int statementNumber, std::string postfixExpression) {
    Statement *statement = this->entityManager->getStatementByStatementNumber(statementNumber);
    this->validateEntityExists(statement);

    if (statement->getEntityType() != EntityType::ASSIGN_STATEMENT) {
        throw std::runtime_error("Statement is not an assign statement, cannot store postfix expressions");
    }

    this->patternManager->storeAssignStatementPostfixExpression((AssignStatement *)statement, new std::string(postfixExpression));
}

void PopulateFacade::storeCallsRelationship(std::string firstProcedureName, std::string secondProcedureName) {
    Procedure *firstProcedure = this->entityManager->getProcedureByProcedureName(firstProcedureName);
    Procedure *secondProcedure = this->entityManager->getProcedureByProcedureName(secondProcedureName);
    this->validateEntityExists(firstProcedure);
    this->validateEntityExists(secondProcedure);
    this->relationshipManager->storeRelationship(std::make_shared<CallsRelationship>(firstProcedure, secondProcedure));
}

void PopulateFacade::storeCallsStarRelationship(std::string firstProcedureName, std::string secondProcedureName) {
    Procedure *firstProcedure = this->entityManager->getProcedureByProcedureName(firstProcedureName);
    Procedure *secondProcedure = this->entityManager->getProcedureByProcedureName(secondProcedureName);
    this->validateEntityExists(firstProcedure);
    this->validateEntityExists(secondProcedure);
    this->relationshipManager->storeRelationship(std::make_shared<CallsStarRelationship>(firstProcedure, secondProcedure));
}

void PopulateFacade::validateEntityExists(Entity *entity) {
    if (entity == nullptr) {
        throw std::runtime_error("Entity does not exist in PKB and the relationship cannot be added, please populate PKB with all entities before storing relationships.");
    }
}
