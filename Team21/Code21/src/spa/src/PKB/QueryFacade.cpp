//
// Created by Aiken Wong on 4/2/23.
//

#include "QueryFacade.h"

#include <memory>

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

QueryFacade::QueryFacade(EntityManager *entityManager, RelationshipManager *relationshipManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
}

std::vector<AssignStatement *> *QueryFacade::getAllAssignStatements() {
    return (std::vector<AssignStatement *> *)this->entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
}

std::vector<IfStatement *> *QueryFacade::getAllIfStatements() {
    return (std::vector<IfStatement *> *)this->entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
}

std::vector<WhileStatement *> *QueryFacade::getAllWhileStatements() {
    return (std::vector<WhileStatement *> *)this->entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
}

std::vector<CallStatement *> *QueryFacade::getAllCallStatements() {
    return (std::vector<CallStatement *> *)this->entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
}

std::vector<ReadStatement *> *QueryFacade::getAllReadStatements() {
    return (std::vector<ReadStatement *> *)this->entityManager->getEntitiesByType(EntityType::READ_STATEMENT);
}

std::vector<PrintStatement *> *QueryFacade::getAllPrintStatements() {
    return (std::vector<PrintStatement *> *)this->entityManager->getEntitiesByType(EntityType::PRINT_STATEMENT);
}
std::vector<Procedure *> *QueryFacade::getAllProcedures() {
    return (std::vector<Procedure *> *)this->entityManager->getEntitiesByType(EntityType::PROCEDURE);
}

std::vector<Variable *> *QueryFacade::getAllVariables() {
    return (std::vector<Variable *> *)this->entityManager->getEntitiesByType(EntityType::VARIABLE);
}

std::vector<Constant *> *QueryFacade::getAllConstants() {
    return (std::vector<Constant *> *)this->entityManager->getEntitiesByType(EntityType::CONSTANT);
}

std::vector<Statement *> *QueryFacade::getAllStatements() {
    return (std::vector<Statement *> *)this->entityManager->getEntitiesByType(EntityType::STATEMENT);
}

std::vector<ParentRelationship *> *QueryFacade::getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ParentRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::PARENT, leftEntityType, rightEntityType);
}

std::vector<FollowsRelationship *> *QueryFacade::getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<FollowsRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS, leftEntityType, rightEntityType);
}

std::vector<ModifiesRelationship *> *QueryFacade::getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ModifiesRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::MODIFIES, leftEntityType, rightEntityType);
}

std::vector<UsesRelationship *> *QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<UsesRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::USES, leftEntityType, rightEntityType);
}

std::vector<ParentStarRelationship *> *QueryFacade::getParentStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ParentStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::PARENT_STAR, leftEntityType, rightEntityType);
}

std::vector<FollowsStarRelationship *> *QueryFacade::getFollowsStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<FollowsStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS_STAR, leftEntityType, rightEntityType);
}

std::vector<CallsRelationship *> *QueryFacade::getAllCallsRelationships() {
    return (std::vector<CallsRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::CALLS, EntityType::PROCEDURE, EntityType::PROCEDURE);
}

std::vector<CallsStarRelationship *> *QueryFacade::getAllCallsStarRelationships() {
    return (std::vector<CallsStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::CALLS_STAR, EntityType::PROCEDURE, EntityType::PROCEDURE);
}

ModifiesRelationship *QueryFacade::getStatementModifiesVariableRelationship(int statementNumber, std::string variableName) {
    Statement *statement = entityManager->getStatementByStatementNumber(statementNumber);
    Variable *variable = entityManager->getVariableByVariableName(variableName);
    if (statement == nullptr || variable == nullptr) {
        return nullptr;
    }
    ModifiesRelationship relationship = ModifiesRelationship(statement, variable);
    return (ModifiesRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

ModifiesRelationship *QueryFacade::getProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = entityManager->getProcedureByProcedureName(procedureName);
    Variable *variable = entityManager->getVariableByVariableName(variableName);
    if (procedure == nullptr || variable == nullptr) {
        return nullptr;
    }
    ModifiesRelationship relationship = ModifiesRelationship(procedure, variable);
    return (ModifiesRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

UsesRelationship *QueryFacade::getStatementUsesVariableRelationship(int statementNumber, std::string variableName) {
    Statement *statement = entityManager->getStatementByStatementNumber(statementNumber);
    Variable *variable = entityManager->getVariableByVariableName(variableName);
    if (statement == nullptr || variable == nullptr) {
        return nullptr;
    }
    UsesRelationship relationship = UsesRelationship(statement, variable);
    return (UsesRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

UsesRelationship *QueryFacade::getProcedureUsesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = entityManager->getProcedureByProcedureName(procedureName);
    Variable *variable = entityManager->getVariableByVariableName(variableName);
    if (procedure == nullptr || variable == nullptr) {
        return nullptr;
    }
    UsesRelationship relationship = UsesRelationship(procedure, variable);
    return (UsesRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

ParentRelationship *QueryFacade::getParentRelationship(int parentStatementNumber, int childStatementNumber) {
    Statement *parentStatement = entityManager->getStatementByStatementNumber(parentStatementNumber);
    Statement *childStatement = entityManager->getStatementByStatementNumber(childStatementNumber);
    if (parentStatement == nullptr || childStatement == nullptr) {
        return nullptr;
    }
    ParentRelationship relationship = ParentRelationship(parentStatement, childStatement);
    return (ParentRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

FollowsRelationship *QueryFacade::getFollowsRelationship(int firstStatementNumber, int secondStatementNumber) {
    Statement *firstStatement = entityManager->getStatementByStatementNumber(firstStatementNumber);
    Statement *secondStatement = entityManager->getStatementByStatementNumber(secondStatementNumber);
    if (firstStatement == nullptr || secondStatement == nullptr) {
        return nullptr;
    }
    FollowsRelationship relationship = FollowsRelationship(firstStatement, secondStatement);
    return (FollowsRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

ParentStarRelationship *QueryFacade::getParentStarRelationship(int parentStatementNumber, int childStatementNumber) {
    Statement *parentStatement = entityManager->getStatementByStatementNumber(parentStatementNumber);
    Statement *childStatement = entityManager->getStatementByStatementNumber(childStatementNumber);
    if (parentStatement == nullptr || childStatement == nullptr) {
        return nullptr;
    }
    ParentStarRelationship relationship = ParentStarRelationship(parentStatement, childStatement);
    return (ParentStarRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

FollowsStarRelationship *QueryFacade::getFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber) {
    Statement *firstStatement = entityManager->getStatementByStatementNumber(firstStatementNumber);
    Statement *secondStatement = entityManager->getStatementByStatementNumber(secondStatementNumber);
    if (firstStatement == nullptr || secondStatement == nullptr) {
        return nullptr;
    }
    FollowsStarRelationship relationship = FollowsStarRelationship(firstStatement, secondStatement);
    return (FollowsStarRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

CallsRelationship *QueryFacade::getCallsRelationship(std::string callerName, std::string calleeName) {
    Procedure *callerProcedure = entityManager->getProcedureByProcedureName(callerName);
    Procedure *calleeProcedure = entityManager->getProcedureByProcedureName(calleeName);
    if (callerProcedure == nullptr || calleeProcedure == nullptr) {
        return nullptr;
    }
    CallsRelationship relationship = CallsRelationship(callerProcedure, calleeProcedure);
    return (CallsRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}

CallsStarRelationship *QueryFacade::getCallsStarRelationship(std::string callerName, std::string calleeName) {
    Procedure *callerProcedure = entityManager->getProcedureByProcedureName(callerName);
    Procedure *calleeProcedure = entityManager->getProcedureByProcedureName(calleeName);
    if (callerProcedure == nullptr || calleeProcedure == nullptr) {
        return nullptr;
    }
    CallsStarRelationship relationship = CallsStarRelationship(callerProcedure, calleeProcedure);
    return (CallsStarRelationship *)this->relationshipManager->getRelationshipIfExist(&relationship);
}
