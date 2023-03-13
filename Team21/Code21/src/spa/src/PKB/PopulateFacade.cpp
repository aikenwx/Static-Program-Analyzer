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

PopulateFacade::PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager, PatternManager *patternManager, CFGManager *cfgManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
    this->patternManager = patternManager;
    this->cfgManager = cfgManager;
}

void PopulateFacade::storeAssignmentStatement(int statementNumber) {
    this->entityManager->storeEntity(new AssignStatement(statementNumber));
}

void PopulateFacade::storeCallStatement(int statementNumber) {
    this->entityManager->storeEntity(new CallStatement(statementNumber));
}

void PopulateFacade::storeIfStatement(int statementNumber) {
    this->entityManager->storeEntity(new IfStatement(statementNumber));
}

void PopulateFacade::storePrintStatement(int statementNumber) {
    this->entityManager->storeEntity(new PrintStatement(statementNumber));
}

void PopulateFacade::storeReadStatement(int statementNumber) {
    this->entityManager->storeEntity(new ReadStatement(statementNumber));
}

void PopulateFacade::storeWhileStatement(int statementNumber) {
    this->entityManager->storeEntity(new WhileStatement(statementNumber));
}

void PopulateFacade::storeConstant(int constantValue) {
    this->entityManager->storeEntity(new Constant(constantValue));
}

void PopulateFacade::storeVariable(std::string variableName) {
    this->entityManager->storeEntity(new Variable(new std::string(variableName)));
}

void PopulateFacade::storeProcedure(std::string procedureName) {
    this->entityManager->storeEntity(new Procedure(new std::string(procedureName)));
}

void PopulateFacade::storeStatementModifiesVariableRelationship(int statementNumber, std::string variableName) {
    auto statementKey = EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
    auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
    Statement *statement = (Statement *)this->entityManager->getEntity(statementKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);
    this->validateEntityExists(statement);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(new ModifiesRelationship(statement, variable));
}

void PopulateFacade::storeStatementUsesVariableRelationship(int statementNumber, std::string variableName) {
    auto statementKey = EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
    auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
    Statement *statement = (Statement *)this->entityManager->getEntity(statementKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);
    this->validateEntityExists(statement);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(new UsesRelationship(statement, variable));
}

void PopulateFacade::storeFollowsRelationship(int firstStatementNumber, int secondStatementNumber) {
    auto firstStatementKey = EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
    auto secondStatementKey = EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);
    Statement *firstStatement = (Statement *)this->entityManager->getEntity(firstStatementKey);
    Statement *secondStatement = (Statement *)this->entityManager->getEntity(secondStatementKey);
    this->validateEntityExists(firstStatement);
    this->validateEntityExists(secondStatement);

    this->relationshipManager->storeRelationship(new FollowsRelationship(firstStatement, secondStatement));
}

void PopulateFacade::storeParentRelationship(int parentStatementNumber, int childStatementNumber) {
    auto parentStatementKey = EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
    auto childStatementKey = EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);
    Statement *parentStatement = (Statement *)this->entityManager->getEntity(parentStatementKey);
    Statement *childStatement = (Statement *)this->entityManager->getEntity(childStatementKey);
    this->validateEntityExists(parentStatement);
    this->validateEntityExists(childStatement);
    this->relationshipManager->storeRelationship(new ParentRelationship(parentStatement, childStatement));
}

void PopulateFacade::storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName) {
    auto procedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
    auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
    Procedure *procedure = (Procedure *)this->entityManager->getEntity(procedureKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);
    this->validateEntityExists(procedure);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(new ModifiesRelationship(procedure, variable));
}

void PopulateFacade::storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName) {
    auto procedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
    auto variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);
    Procedure *procedure = (Procedure *)this->entityManager->getEntity(procedureKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);
    this->validateEntityExists(procedure);
    this->validateEntityExists(variable);
    this->relationshipManager->storeRelationship(new UsesRelationship(procedure, variable));
}

void PopulateFacade::storeParentStarRelationship(int parentStatementNumber, int childStatementNumber) {
    auto parentStatementKey = EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
    auto childStatementKey = EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);
    Statement *parentStatement = (Statement *)this->entityManager->getEntity(parentStatementKey);
    Statement *childStatement = (Statement *)this->entityManager->getEntity(childStatementKey);
    this->validateEntityExists(parentStatement);
    this->validateEntityExists(childStatement);
    this->relationshipManager->storeRelationship(new ParentStarRelationship(parentStatement, childStatement));
}

void PopulateFacade::storeFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber) {
    auto firstStatementKey = EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
    auto secondStatementKey = EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);
    Statement *firstStatement = (Statement *)this->entityManager->getEntity(firstStatementKey);
    Statement *secondStatement = (Statement *)this->entityManager->getEntity(secondStatementKey);
    this->validateEntityExists(firstStatement);
    this->validateEntityExists(secondStatement);
    this->relationshipManager->storeRelationship(new FollowsStarRelationship(firstStatement, secondStatement));
}

void PopulateFacade::storeAssignStatementPostfixExpression(int statementNumber, std::string postfixExpression) {
    auto statementKey = EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
    Statement *statement = (Statement *)this->entityManager->getEntity(statementKey);

    this->validateEntityExists(statement);

    if (!(statement->getEntityType() == AssignStatement::getEntityTypeStatic())) {
        throw std::runtime_error("Statement is not an assign statement, cannot store postfix expressions");
    }

    this->patternManager->storeAssignStatementPostfixExpression((AssignStatement *)statement, new std::string(postfixExpression));
}

void PopulateFacade::storeCallsRelationship(std::string firstProcedureName, std::string secondProcedureName) {
    auto firstProcedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &firstProcedureName);
    auto secondProcedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &secondProcedureName);
    Procedure *firstProcedure = (Procedure *)this->entityManager->getEntity(firstProcedureKey);
    Procedure *secondProcedure = (Procedure *)this->entityManager->getEntity(secondProcedureKey);
    this->validateEntityExists(firstProcedure);
    this->validateEntityExists(secondProcedure);
    this->relationshipManager->storeRelationship(new CallsRelationship(firstProcedure, secondProcedure));
}

void PopulateFacade::storeCallsStarRelationship(std::string firstProcedureName, std::string secondProcedureName) {
    auto firstProcedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &firstProcedureName);
    auto secondProcedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &secondProcedureName);
    Procedure *firstProcedure = (Procedure *)this->entityManager->getEntity(firstProcedureKey);
    Procedure *secondProcedure = (Procedure *)this->entityManager->getEntity(secondProcedureKey);
    this->validateEntityExists(firstProcedure);
    this->validateEntityExists(secondProcedure);
    this->relationshipManager->storeRelationship(new CallsStarRelationship(firstProcedure, secondProcedure));
}

void PopulateFacade::validateEntityExists(Entity *entity) {
    if (entity == nullptr) {
        throw std::runtime_error("Entity does not exist in PKB and the relationship cannot be added, please populate PKB with all entities before storing relationships.");
    }
}

void PopulateFacade::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
    this->cfgManager->storeCFG(cfg);
}
