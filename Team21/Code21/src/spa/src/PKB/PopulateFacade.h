//
// Created by Aiken Wong on 4/2/23.
//

#include <memory>
#include <string>

#include "PKB/CFGManager.h"
#include "PKB/EntityManager.h"
#include "PKB/PatternManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "sp/cfg/cfg.h"

#ifndef SPA_POPULATEFACADE_H
#define SPA_POPULATEFACADE_H

class PopulateFacade {
   private:
    RelationshipManager *relationshipManager;
    EntityManager *entityManager;
    PatternManager *patternManager;
    CFGManager *cfgManager;

   public:
    PopulateFacade(EntityManager *entityManager, RelationshipManager *relationshipManager, PatternManager *patternManager, CFGManager *cfgManager);

    void storeAssignmentStatement(int statementNumber);
    void storeCallStatement(int statementNumber);
    void storeIfStatement(int statementNumber);
    void storePrintStatement(int statementNumber);
    void storeReadStatement(int statementNumber);
    void storeWhileStatement(int statementNumber);
    void storeConstant(int constantValue);
    void storeVariable(const std::string& variableName);
    void storeProcedure(const std::string& procedureName);

    void storeStatementModifiesVariableRelationship(int statementNumber, std::string variableName);
    void storeProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName);
    void storeStatementUsesVariableRelationship(int statementNumber, std::string variableName);
    void storeProcedureUsesVariableRelationship(std::string procedureName, std::string variableName);
    void storeParentRelationship(int parentStatementNumber, int childStatementNumber);
    void storeFollowsRelationship(int firstStatementNumber, int secondStatementNumber);
    void storeParentStarRelationship(int parentStatementNumber, int childStatementNumber);
    void storeFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber);
    void storeCallsRelationship(std::string caller, std::string callee);
    void storeCallsStarRelationship(std::string caller, std::string callee);

    void storeAssignStatementPostfixExpression(int statementNumber, const std::string& postfixExpression);
    void storeCallStatementProcedureName(int statementNumber, const std::string& procedureName);

    void storeWhileStatementConditionVariable(int statementNumber, std::string variableName);
    void storeIfStatementConditionVariable(int statementNumber, std::string variableName);

    void storeCFG(std::shared_ptr<cfg::CFG> cfg);

   private:
    static void validateEntityExists(Entity *entity);
};

#endif  // SPA_POPULATEFACADE_H
