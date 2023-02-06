//
// Created by Aiken Wong on 4/2/23.
//

#include "QueryFacade.h"

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"

#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"


QueryFacade::QueryFacade(EntityManager *entityManager, RelationshipManager *relationshipManager) {
    this->entityManager = entityManager;
    this->relationshipManager = relationshipManager;
}

QueryFacade::~QueryFacade() {}

std::vector<AssignStatement *> *QueryFacade::getAllAssignStatements() {
    return (std::vector<AssignStatement *> *) this->entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
}

std::vector<IfStatement *> *QueryFacade::getAllIfStatements() {
    return (std::vector<IfStatement *> *) this->entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
}

std::vector<WhileStatement *> *QueryFacade::getAllWhileStatements() {
    return (std::vector<WhileStatement *> *) this->entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
}

std::vector<CallStatement *> *QueryFacade::getAllCallStatements() {
    return (std::vector<CallStatement *> *) this->entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
}

std::vector<ReadStatement *> *QueryFacade::getAllReadStatements() {
    return (std::vector<ReadStatement *> *) this->entityManager->getEntitiesByType(EntityType::READ_STATEMENT);
}

std::vector<PrintStatement *> *QueryFacade::getAllPrintStatements() {
    return (std::vector<PrintStatement *> *) this->entityManager->getEntitiesByType(EntityType::PRINT_STATEMENT);
}

std::vector<Procedure *> *QueryFacade::getAllProcedures() {
    return (std::vector<Procedure *> *) this->entityManager->getEntitiesByType(EntityType::PROCEDURE);
}

std::vector<Variable *> *QueryFacade::getAllVariables() {
    return (std::vector<Variable *> *) this->entityManager->getEntitiesByType(EntityType::VARIABLE);
}

std::vector<Constant *> *QueryFacade::getAllConstants() {
    return (std::vector<Constant *> *) this->entityManager->getEntitiesByType(EntityType::CONSTANT);
}

std::vector<Statement *> *QueryFacade::getAllStatements() {
    return (std::vector<Statement *> *) this->entityManager->getEntitiesByType(EntityType::STATEMENT);
}

std::vector<ParentRelationship *> *QueryFacade::getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ParentRelationship *> *) this->relationshipManager->getRelationshipsByTypes(
            RelationshipType::PARENT, leftEntityType, rightEntityType);
}

std::vector<FollowsRelationship *> *QueryFacade::getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<FollowsRelationship *> *) this->relationshipManager->getRelationshipsByTypes(
            RelationshipType::FOLLOWS, leftEntityType, rightEntityType);
}

std::vector<ModifiesRelationship *> *QueryFacade::getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<ModifiesRelationship *> *) this->relationshipManager->getRelationshipsByTypes(
            RelationshipType::MODIFIES, leftEntityType, rightEntityType);
}

std::vector<UsesRelationship *> *QueryFacade::getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType) {
    return (std::vector<UsesRelationship *> *) this->relationshipManager->getRelationshipsByTypes(
            RelationshipType::USES, leftEntityType, rightEntityType);
}





