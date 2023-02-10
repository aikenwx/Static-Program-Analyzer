//
// Created by Aiken Wong on 4/2/23.
//

#include "PopulateFacade.h"

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
#include "PKBStorageClasses/EntityFactory.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipFactory.h"

PopulateFacade::PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
    this->entityFactory = new EntityFactory();
    this->relationshipFactory = new RelationshipFactory();
}

PopulateFacade::~PopulateFacade() {
    delete this->entityFactory;
    delete this->relationshipFactory;
}

void PopulateFacade::storeAssignmentStatement(int statementNumber) {
    AssignStatement *assignStatement = (AssignStatement *)this->entityFactory->createEntity(EntityType::ASSIGN_STATEMENT, statementNumber);
    this->entityManager->storeEntity(assignStatement);
}

void PopulateFacade::storeCallStatement(int statementNumber) {
    CallStatement *callStatement = (CallStatement *)this->entityFactory->createEntity(EntityType::CALL_STATEMENT, statementNumber);
    this->entityManager->storeEntity(callStatement);
}

void PopulateFacade::storeIfStatement(int statementNumber) {
    IfStatement *ifStatement = (IfStatement *)this->entityFactory->createEntity(EntityType::IF_STATEMENT, statementNumber);
    this->entityManager->storeEntity(ifStatement);
}

void PopulateFacade::storePrintStatement(int statementNumber) {
    PrintStatement *printStatement = (PrintStatement *)this->entityFactory->createEntity(EntityType::PRINT_STATEMENT, statementNumber);
    this->entityManager->storeEntity(printStatement);
}

void PopulateFacade::storeReadStatement(int statementNumber) {
    ReadStatement *readStatement = (ReadStatement *)this->entityFactory->createEntity(EntityType::READ_STATEMENT, statementNumber);
    this->entityManager->storeEntity(readStatement);
}

void PopulateFacade::storeWhileStatement(int statementNumber) {
    WhileStatement *whileStatement = (WhileStatement *)this->entityFactory->createEntity(EntityType::WHILE_STATEMENT, statementNumber);
    this->entityManager->storeEntity(whileStatement);
}

void PopulateFacade::storeConstant(int constantValue) {
    Constant *constant = (Constant *)this->entityFactory->createEntity(EntityType::CONSTANT, constantValue);
    this->entityManager->storeEntity(constant);
}

void PopulateFacade::storeVariable(std::string variableName) {
    Variable *variable = (Variable *)this->entityFactory->createEntity(EntityType::VARIABLE, variableName);
    this->entityManager->storeEntity(variable);
}

void PopulateFacade::storeProcedure(std::string procedureName) {
    Procedure *procedure = (Procedure *)this->entityFactory->createEntity(EntityType::PROCEDURE, procedureName);
    this->entityManager->storeEntity(procedure);
}

void PopulateFacade::storeStatementModifiesVariableRelationship(int statementNumber, EntityType entityTypeOfStatement, std::string variableName) {
    Statement *statement = (Statement *)this->entityFactory->createEntity(entityTypeOfStatement, statementNumber);
    Variable *variable = (Variable *)this->entityFactory->createEntity(EntityType::VARIABLE, variableName);
    ModifiesRelationship *modifiesRelationship = (ModifiesRelationship *)this->relationshipFactory->createRelationship(RelationshipType::MODIFIES, statement, variable);
    this->relationshipManager->storeRelationship(modifiesRelationship);
}

void PopulateFacade::storeStatementUsesVariableRelationship(int statementNumber, EntityType entityTypeOfStatement, std::string variableName) {
    Statement *statement = (Statement *)this->entityFactory->createEntity(entityTypeOfStatement, statementNumber);
    Variable *variable = (Variable *)this->entityFactory->createEntity(EntityType::VARIABLE, variableName);
    UsesRelationship *usesRelationship = (UsesRelationship *)this->relationshipFactory->createRelationship(RelationshipType::USES, statement, variable);
    this->relationshipManager->storeRelationship(usesRelationship);
}

void PopulateFacade::storeFollowsRelationship(int firstStatementNumber, EntityType entityTypeOfFirstStatement, int secondStatementNumber, EntityType entityTypeOfSecondStatement) {
    Statement *statement = (Statement *)this->entityFactory->createEntity(entityTypeOfFirstStatement, firstStatementNumber);
    Statement *statement2 = (Statement *)this->entityFactory->createEntity(entityTypeOfSecondStatement, secondStatementNumber);
    FollowsRelationship *followsRelationship = (FollowsRelationship *)this->relationshipFactory->createRelationship(RelationshipType::FOLLOWS, statement, statement2);
    this->relationshipManager->storeRelationship(followsRelationship);
}

void PopulateFacade::storeParentRelationship(int parentStatementNumber, EntityType entityTypeOfParentStatement, int childStatementNumber, EntityType entityTypeOfChildStatement) {
    Statement *statement = (Statement *)this->entityFactory->createEntity(entityTypeOfParentStatement, parentStatementNumber);
    Statement *statement2 = (Statement *)this->entityFactory->createEntity(entityTypeOfChildStatement, childStatementNumber);
    ParentRelationship *parentRelationship = (ParentRelationship *)this->relationshipFactory->createRelationship(RelationshipType::PARENT, statement, statement2);
    this->relationshipManager->storeRelationship(parentRelationship);
}

void PopulateFacade::storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = (Procedure *)this->entityFactory->createEntity(EntityType::PROCEDURE, procedureName);
    Variable *variable = (Variable *)this->entityFactory->createEntity(EntityType::VARIABLE, variableName);
    ModifiesRelationship *modifiesRelationship = (ModifiesRelationship *)this->relationshipFactory->createRelationship(RelationshipType::MODIFIES, procedure, variable);
    this->relationshipManager->storeRelationship(modifiesRelationship);
}

void PopulateFacade::storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName) {
    Procedure *procedure = (Procedure *)this->entityFactory->createEntity(EntityType::PROCEDURE, procedureName);
    Variable *variable = (Variable *)this->entityFactory->createEntity(EntityType::VARIABLE, variableName);
    UsesRelationship *usesRelationship = (UsesRelationship *)this->relationshipFactory->createRelationship(RelationshipType::USES, procedure, variable);
    this->relationshipManager->storeRelationship(usesRelationship);
}
