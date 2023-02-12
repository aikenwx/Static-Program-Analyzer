//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_QUERYFACADE_H
#define SPA_QUERYFACADE_H

#include <vector>

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
#include "QueryFacade.h"

class QueryFacade {
   private:
    EntityManager* entityManager;
    RelationshipManager* relationshipManager;

   public:
    QueryFacade(EntityManager* entityManager, RelationshipManager* relationshipManager);

    std::vector<AssignStatement*>* getAllAssignStatements();
    std::vector<IfStatement*>* getAllIfStatements();
    std::vector<WhileStatement*>* getAllWhileStatements();
    std::vector<CallStatement*>* getAllCallStatements();
    std::vector<ReadStatement*>* getAllReadStatements();
    std::vector<PrintStatement*>* getAllPrintStatements();
    std::vector<Procedure*>* getAllProcedures();
    std::vector<Variable*>* getAllVariables();
    std::vector<Constant*>* getAllConstants();
    std::vector<Statement*>* getAllStatements();

    std::vector<ParentRelationship*>* getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<FollowsRelationship*>* getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<ModifiesRelationship*>* getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<UsesRelationship*>* getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);

    ModifiesRelationship* getStatementModifiesVariableRelationship(int statementNumber, std::string variableName);
    ModifiesRelationship* getProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName);
    UsesRelationship* getStatementUsesVariableRelationship(int statementNumber, std::string variableName);
    ModifiesRelationship* getProcedureUsesVariableRelationship(std::string procedureName, std::string variableName);
    ParentRelationship* getParentRelationship(int parentStatementNumber, int childStatementNumber);
    ParentRelationship* getFollowsRelationship(int firstStatementNumber, int secondStatementNumber);
};

#endif  // SPA_QUERYFACADE_H
