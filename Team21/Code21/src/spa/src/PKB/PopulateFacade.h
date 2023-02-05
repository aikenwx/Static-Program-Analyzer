//
// Created by Aiken Wong on 4/2/23.
//

#include <string>
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityFactory.h"
#include "PKBStorageClasses/RelationshipFactory.h"

#ifndef SPA_POPULATEFACADE_H
#define SPA_POPULATEFACADE_H



class PopulateFacade {

private:
    RelationshipManager *relationshipManager;
    EntityManager *entityManager;
    EntityFactory *entityFactory;
    RelationshipFactory *relationshipFactory;

public:
    PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager);
    ~PopulateFacade();

    void storeAssignmentStatement(int statementNumber);
    void storeCallStatement(int statementNumber);
    void storeIfStatement(int statementNumber);
    void storePrintStatement(int statementNumber);
    void storeReadStatement(int statementNumber);
    void storeWhileStatement(int statementNumber);
    void storeConstant(int constantValue);
    void storeVariable(std::string variableName);
    void storeProcedure(std::string procedureName);

    void storeStatementModifiesVariableRelationship(int statementNumber, EntityType entityTypeOfStatement, std::string variableName);
    void storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName);
    void storeStatementUsesVariableRelationship(int statementNumber, EntityType entityTypeOfStatement, std::string variableName);
    void storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName);
    void storeParentRelationship(int parentStatementNumber, EntityType entityTypeOfParent, int childStatementNumber, EntityType entityTypeOfChild);
    void storeFollowsRelationship(int firstStatementNumber, EntityType entityTypeOfFirstStatement, int secondStatementNumber, EntityType entityTypeOfSecondStatement);
};


#endif //SPA_POPULATEFACADE_H
