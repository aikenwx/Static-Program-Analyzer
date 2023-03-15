//
// Created by Aiken Wong on 12/2/23.
//

#ifndef SPA_PATTERNMANAGER_H
#define SPA_PATTERNMANAGER_H

#include <set>
#include <string>
#include <unordered_map>

#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "../PKBStorageClasses/EntityClasses/IfStatement.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"

class PatternManager {
   private:
    std::unordered_map<std::string, std::shared_ptr<std::set<WhileStatement*>>> usedConditionVariableToWhileStatementStore;
    std::unordered_map<std::string, std::shared_ptr<std::set<IfStatement*>>> usedConditionVariableToIfStatementStore;

   public:
    PatternManager() = default;
    void storeAssignStatementPostfixExpression(AssignStatement* assignStatement, std::string* postfixExpression);

    void storeWhileStatementConditionVariable(WhileStatement* whileStatement, std::string* conditionVariable);

    void storeIfStatementConditionVariable(IfStatement* ifStatement, std::string* conditionVariable);

    std::set<WhileStatement*>* getWhileStatementsByConditionVariable(std::string* conditionVariable);

    std::set<IfStatement*>* getIfStatementsByConditionVariable(std::string* conditionVariable);
};

#endif  // SPA_PATTERNMANAGER_H
