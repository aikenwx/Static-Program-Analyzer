//
// Created by Aiken Wong on 12/2/23.
//

#ifndef SPA_PATTERNMANAGER_H
#define SPA_PATTERNMANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "../PKBStorageClasses/EntityClasses/IfStatement.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"

class PatternManager {
   private:
    std::unordered_map<std::string, std::shared_ptr<std::unordered_set<WhileStatement*>>> usedConditionVariableToWhileStatementStore;
    std::unordered_map<std::string, std::shared_ptr<std::unordered_set<IfStatement*>>> usedConditionVariableToIfStatementStore;

   public:
    PatternManager() = default;
    static void storeAssignStatementPostfixExpression(
        AssignStatement* assignStatement, std::string* postfixExpression);

    void storeWhileStatementConditionVariable(WhileStatement* whileStatement, std::string* conditionVariable);

    void storeIfStatementConditionVariable(IfStatement* ifStatement, std::string* conditionVariable);

    auto getWhileStatementsByConditionVariable(std::string* conditionVariable)
        -> std::unordered_set<WhileStatement*>*;

    auto getIfStatementsByConditionVariable(std::string* conditionVariable)
        -> std::unordered_set<IfStatement*>*;
};

#endif  // SPA_PATTERNMANAGER_H
