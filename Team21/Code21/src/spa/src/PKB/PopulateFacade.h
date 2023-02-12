//
// Created by Aiken Wong on 4/2/23.
//

#include <memory>
#include <string>

#include "PKB/EntityManager.h"
#include "PKB/PatternManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

#ifndef SPA_POPULATEFACADE_H
#define SPA_POPULATEFACADE_H

class PopulateFacade {
   private:
    RelationshipManager *relationshipManager;
    EntityManager *entityManager;
    PatternManager *patternManager;

   public:
    PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager, PatternManager *patternManager);

    void storeAssignmentStatement(int statementNumber);
    void storeCallStatement(int statementNumber);
    void storeIfStatement(int statementNumber);
    void storePrintStatement(int statementNumber);
    void storeReadStatement(int statementNumber);
    void storeWhileStatement(int statementNumber);
    void storeConstant(int constantValue);
    void storeVariable(std::string variableName);
    void storeProcedure(std::string procedureName);

    void storeStatementModifiesVariableRelationship(int statementNumber, std::string variableName);
    void storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName);
    void storeStatementUsesVariableRelationship(int statementNumber, std::string variableName);
    void storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName);
    void storeParentRelationship(int parentStatementNumber, int childStatementNumber);
    void storeFollowsRelationship(int firstStatementNumber, int secondStatementNumber);

    void storeAssignStatementPostfixExpression(int statementNumber, std::string *postfixExpression);

   private:
    void validateEntityExists(Entity *entity);
};

#endif  // SPA_POPULATEFACADE_H
