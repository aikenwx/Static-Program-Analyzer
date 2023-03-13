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

QueryFacade::QueryFacade(EntityManager *entityManager, RelationshipManager *relationshipManager, CFGManager *cfgManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
    this->cfgManager = cfgManager;
}

std::vector<AssignStatement *> *QueryFacade::getAllAssignStatements() {
    return (std::vector<AssignStatement *> *)this->entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());
}

std::vector<IfStatement *> *QueryFacade::getAllIfStatements() {
    return (std::vector<IfStatement *> *)this->entityManager->getEntitiesByType(IfStatement::getEntityTypeStatic());
}

std::vector<WhileStatement *> *QueryFacade::getAllWhileStatements() {
    return (std::vector<WhileStatement *> *)this->entityManager->getEntitiesByType(WhileStatement::getEntityTypeStatic());
}

std::vector<CallStatement *> *QueryFacade::getAllCallStatements() {
    return (std::vector<CallStatement *> *)this->entityManager->getEntitiesByType(CallStatement::getEntityTypeStatic());
}

std::vector<ReadStatement *> *QueryFacade::getAllReadStatements() {
    return (std::vector<ReadStatement *> *)this->entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());
}

std::vector<PrintStatement *> *QueryFacade::getAllPrintStatements() {
    return (std::vector<PrintStatement *> *)this->entityManager->getEntitiesByType(PrintStatement::getEntityTypeStatic());
}
std::vector<Procedure *> *QueryFacade::getAllProcedures() {
    return (std::vector<Procedure *> *)this->entityManager->getEntitiesByType(Procedure::getEntityTypeStatic());
}

std::vector<Variable *> *QueryFacade::getAllVariables() {
    return (std::vector<Variable *> *)this->entityManager->getEntitiesByType(Variable::getEntityTypeStatic());
}

std::vector<Constant *> *QueryFacade::getAllConstants() {
    return (std::vector<Constant *> *)this->entityManager->getEntitiesByType(Constant::getEntityTypeStatic());
}

std::vector<Statement *> *QueryFacade::getAllStatements() {
    return (std::vector<Statement *> *)this->entityManager->getEntitiesByType(Statement::getEntityTypeStatic());
}

std::vector<ParentRelationship *> *QueryFacade::getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ParentRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        ParentRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<FollowsRelationship *> *QueryFacade::getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<FollowsRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        FollowsRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<ModifiesRelationship *> *QueryFacade::getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ModifiesRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        ModifiesRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<UsesRelationship *> *QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<UsesRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        UsesRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<ParentStarRelationship *> *QueryFacade::getParentStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ParentStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        ParentStarRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<FollowsStarRelationship *> *QueryFacade::getFollowsStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<FollowsStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        FollowsStarRelationship::getRelationshipTypeStatic(), leftEntityType, rightEntityType);
}

std::vector<CallsRelationship *> *QueryFacade::getAllCallsRelationships() {
    return (std::vector<CallsRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        CallsRelationship::getRelationshipTypeStatic(), Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());
}

std::vector<CallsStarRelationship *> *QueryFacade::getAllCallsStarRelationships() {
    return (std::vector<CallsStarRelationship *> *)this->relationshipManager->getRelationshipsByTypes(

        CallsStarRelationship::getRelationshipTypeStatic(), Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());
}

ModifiesRelationship *QueryFacade::getStatementModifiesVariableRelationship(int statementNumber, std::string variableName) {
    EntityKey statementKey = EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
    EntityKey variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

    Statement *statement = (Statement *)this->entityManager->getEntity(statementKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);

    if (statement == nullptr || variable == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(), &statementKey, &variableKey);

    return (ModifiesRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

ModifiesRelationship *QueryFacade::getProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName) {
    EntityKey procedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
    EntityKey variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

    Procedure *procedure = (Procedure *)this->entityManager->getEntity(procedureKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);

    if (procedure == nullptr || variable == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&ModifiesRelationship::getRelationshipTypeStatic(), &procedureKey, &variableKey);

    return (ModifiesRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

UsesRelationship *QueryFacade::getStatementUsesVariableRelationship(int statementNumber, std::string variableName) {
    EntityKey statementKey = EntityKey(&Statement::getEntityTypeStatic(), statementNumber);
    EntityKey variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

    Statement *statement = (Statement *)this->entityManager->getEntity(statementKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);

    if (statement == nullptr || variable == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(), &statementKey, &variableKey);

    return (UsesRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

UsesRelationship *QueryFacade::getProcedureUsesVariableRelationship(std::string procedureName, std::string variableName) {
    EntityKey procedureKey = EntityKey(&Procedure::getEntityTypeStatic(), &procedureName);
    EntityKey variableKey = EntityKey(&Variable::getEntityTypeStatic(), &variableName);

    Procedure *procedure = (Procedure *)this->entityManager->getEntity(procedureKey);
    Variable *variable = (Variable *)this->entityManager->getEntity(variableKey);

    if (procedure == nullptr || variable == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&UsesRelationship::getRelationshipTypeStatic(), &procedureKey, &variableKey);

    return (UsesRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}
ParentRelationship *QueryFacade::getParentRelationship(int parentStatementNumber, int childStatementNumber) {
    EntityKey parentStatementKey = EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
    EntityKey childStatementKey = EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);

    Statement *parentStatement = (Statement *)this->entityManager->getEntity(parentStatementKey);
    Statement *childStatement = (Statement *)this->entityManager->getEntity(childStatementKey);

    if (parentStatement == nullptr || childStatement == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&ParentRelationship::getRelationshipTypeStatic(), &parentStatementKey, &childStatementKey);

    return (ParentRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

FollowsRelationship *QueryFacade::getFollowsRelationship(int firstStatementNumber, int secondStatementNumber) {
    EntityKey firstStatementKey = EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
    EntityKey secondStatementKey = EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);

    Statement *firstStatement = (Statement *)this->entityManager->getEntity(firstStatementKey);
    Statement *secondStatement = (Statement *)this->entityManager->getEntity(secondStatementKey);

    if (firstStatement == nullptr || secondStatement == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&FollowsRelationship::getRelationshipTypeStatic(), &firstStatementKey, &secondStatementKey);

    return (FollowsRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

ParentStarRelationship *QueryFacade::getParentStarRelationship(int parentStatementNumber, int childStatementNumber) {
    EntityKey parentStatementKey = EntityKey(&Statement::getEntityTypeStatic(), parentStatementNumber);
    EntityKey childStatementKey = EntityKey(&Statement::getEntityTypeStatic(), childStatementNumber);

    Statement *parentStatement = (Statement *)this->entityManager->getEntity(parentStatementKey);
    Statement *childStatement = (Statement *)this->entityManager->getEntity(childStatementKey);

    if (parentStatement == nullptr || childStatement == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&ParentStarRelationship::getRelationshipTypeStatic(), &parentStatementKey, &childStatementKey);

    return (ParentStarRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

FollowsStarRelationship *QueryFacade::getFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber) {
    EntityKey firstStatementKey = EntityKey(&Statement::getEntityTypeStatic(), firstStatementNumber);
    EntityKey secondStatementKey = EntityKey(&Statement::getEntityTypeStatic(), secondStatementNumber);

    Statement *firstStatement = (Statement *)this->entityManager->getEntity(firstStatementKey);
    Statement *secondStatement = (Statement *)this->entityManager->getEntity(secondStatementKey);

    if (firstStatement == nullptr || secondStatement == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&FollowsStarRelationship::getRelationshipTypeStatic(), &firstStatementKey, &secondStatementKey);

    return (FollowsStarRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

CallsRelationship *QueryFacade::getCallsRelationship(std::string callerName, std::string calleeName) {
    EntityKey callerKey = EntityKey(&Procedure::getEntityTypeStatic(), &callerName);
    EntityKey calleeKey = EntityKey(&Procedure::getEntityTypeStatic(), &calleeName);

    Procedure *callerProcedure = (Procedure *)this->entityManager->getEntity(callerKey);
    Procedure *calleeProcedure = (Procedure *)this->entityManager->getEntity(calleeKey);

    if (callerProcedure == nullptr || calleeProcedure == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&CallsRelationship::getRelationshipTypeStatic(), &callerKey, &calleeKey);

    return (CallsRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

CallsStarRelationship *QueryFacade::getCallsStarRelationship(std::string callerName, std::string calleeName) {
    EntityKey callerKey = EntityKey(&Procedure::getEntityTypeStatic(), &callerName);
    EntityKey calleeKey = EntityKey(&Procedure::getEntityTypeStatic(), &calleeName);

    Procedure *callerProcedure = (Procedure *)this->entityManager->getEntity(callerKey);
    Procedure *calleeProcedure = (Procedure *)this->entityManager->getEntity(calleeKey);

    if (callerProcedure == nullptr || calleeProcedure == nullptr) {
        return nullptr;
    }

    RelationshipKey relationshipKey = RelationshipKey(&CallsStarRelationship::getRelationshipTypeStatic(), &callerKey, &calleeKey);

    return (CallsStarRelationship *)this->relationshipManager->getRelationship(relationshipKey);
}

cfg::CFG *QueryFacade::getCFG() {
    return this->cfgManager->getCFG();
}
