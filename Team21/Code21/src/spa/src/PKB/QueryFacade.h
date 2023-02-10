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

    std::vector<std::shared_ptr<AssignStatement>>* getAllAssignStatements();
    std::vector<std::shared_ptr<IfStatement>>* getAllIfStatements();
    std::vector<std::shared_ptr<WhileStatement>>* getAllWhileStatements();
    std::vector<std::shared_ptr<CallStatement>>* getAllCallStatements();
    std::vector<std::shared_ptr<ReadStatement>>* getAllReadStatements();
    std::vector<std::shared_ptr<PrintStatement>>* getAllPrintStatements();
    std::vector<std::shared_ptr<Procedure>>* getAllProcedures();
    std::vector<std::shared_ptr<Variable>>* getAllVariables();
    std::vector<std::shared_ptr<Constant>>* getAllConstants();
    std::vector<std::shared_ptr<Statement>>* getAllStatements();

    std::vector<std::shared_ptr<ParentRelationship>>* getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<std::shared_ptr<FollowsRelationship>>* getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<std::shared_ptr<ModifiesRelationship>>* getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    std::vector<std::shared_ptr<UsesRelationship>>* getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
};

#endif  // SPA_QUERYFACADE_H
