//
// Created by Aiken Wong on 4/2/23.
//

#include "QueryFacade.h"

#include <iostream>
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
    this->entityManager = std::shared_ptr<EntityManager>(entityManager);
    this->relationshipManager = std::shared_ptr<RelationshipManager>(relationshipManager);
}

std::vector<std::shared_ptr<AssignStatement>> *QueryFacade::getAllAssignStatements() {
    return (std::vector<std::shared_ptr<AssignStatement>> *)this->entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
}

std::vector<std::shared_ptr<IfStatement>> *QueryFacade::getAllIfStatements() {
    return (std::vector<std::shared_ptr<IfStatement>> *)this->entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
}

std::vector<std::shared_ptr<WhileStatement>> *QueryFacade::getAllWhileStatements() {
    return (std::vector<std::shared_ptr<WhileStatement>> *)this->entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
}

std::vector<std::shared_ptr<CallStatement>> *QueryFacade::getAllCallStatements() {
    return (std::vector<std::shared_ptr<CallStatement>> *)this->entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
}

std::vector<std::shared_ptr<ReadStatement>> *QueryFacade::getAllReadStatements() {
    return (std::vector<std::shared_ptr<ReadStatement>> *)this->entityManager->getEntitiesByType(EntityType::READ_STATEMENT);
}

std::vector<std::shared_ptr<PrintStatement>> *QueryFacade::getAllPrintStatements() {
    return (std::vector<std::shared_ptr<PrintStatement>> *)this->entityManager->getEntitiesByType(EntityType::PRINT_STATEMENT);
}
std::vector<std::shared_ptr<Procedure>> *QueryFacade::getAllProcedures() {
    return (std::vector<std::shared_ptr<Procedure>> *)this->entityManager->getEntitiesByType(EntityType::PROCEDURE);
}

std::vector<std::shared_ptr<Variable>> *QueryFacade::getAllVariables() {
    return (std::vector<std::shared_ptr<Variable>> *)this->entityManager->getEntitiesByType(EntityType::VARIABLE);
}

std::vector<std::shared_ptr<Constant>> *QueryFacade::getAllConstants() {
    return (std::vector<std::shared_ptr<Constant>> *)this->entityManager->getEntitiesByType(EntityType::CONSTANT);
}

std::vector<std::shared_ptr<Statement>> *QueryFacade::getAllStatements() {
    return (std::vector<std::shared_ptr<Statement>> *)this->entityManager->getEntitiesByType(EntityType::STATEMENT);
}

std::vector<std::shared_ptr<ParentRelationship>> *QueryFacade::getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<std::shared_ptr<ParentRelationship>> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::PARENT, leftEntityType, rightEntityType);
}

std::vector<std::shared_ptr<FollowsRelationship>> *QueryFacade::getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<std::shared_ptr<FollowsRelationship>> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS, leftEntityType, rightEntityType);
}

std::vector<std::shared_ptr<ModifiesRelationship>> *QueryFacade::getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<std::shared_ptr<ModifiesRelationship>> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::MODIFIES, leftEntityType, rightEntityType);
}

std::vector<std::shared_ptr<UsesRelationship>> *QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<std::shared_ptr<UsesRelationship>> *)this->relationshipManager->getRelationshipsByTypes(
        RelationshipType::USES, leftEntityType, rightEntityType);
}
