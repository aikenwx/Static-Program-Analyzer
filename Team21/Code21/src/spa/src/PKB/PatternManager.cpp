//
// Created by Aiken Wong on 12/2/23.
//

#include "PatternManager.h"

void PatternManager::storeAssignStatementPostfixExpression(AssignStatement *assignStatement, std::string *postfixExpression) {
    assignStatement->setPostfixExpression(postfixExpression);
}

void PatternManager::storeWhileStatementConditionVariable(WhileStatement *whileStatement, std::string *conditionVariable) {
    if (usedConditionVariableToWhileStatementStore.find(*conditionVariable) == usedConditionVariableToWhileStatementStore.end()) {
        usedConditionVariableToWhileStatementStore[*conditionVariable] = std::make_shared<std::unordered_set<WhileStatement *>>();
    }
    usedConditionVariableToWhileStatementStore[*conditionVariable]->insert(whileStatement);
}

void PatternManager::storeIfStatementConditionVariable(IfStatement *ifStatement, std::string *conditionVariable) {
    if (usedConditionVariableToIfStatementStore.find(*conditionVariable) == usedConditionVariableToIfStatementStore.end()) {
        usedConditionVariableToIfStatementStore[*conditionVariable] = std::make_shared<std::unordered_set<IfStatement *>>();
    }
    usedConditionVariableToIfStatementStore[*conditionVariable]->insert(ifStatement);
}

auto PatternManager::getWhileStatementsByConditionVariable(
    std::string *conditionVariable) -> std::unordered_set<WhileStatement *> * {
  if (usedConditionVariableToWhileStatementStore.find(*conditionVariable) ==
      usedConditionVariableToWhileStatementStore.end()) {
    usedConditionVariableToWhileStatementStore[*conditionVariable] =
        std::make_shared<std::unordered_set<WhileStatement *>>();
    return usedConditionVariableToWhileStatementStore[*conditionVariable].get();
  }
  return usedConditionVariableToWhileStatementStore[*conditionVariable].get();
}

auto PatternManager::getIfStatementsByConditionVariable(
    std::string *conditionVariable) -> std::unordered_set<IfStatement *> * {
  if (usedConditionVariableToIfStatementStore.find(*conditionVariable) ==
      usedConditionVariableToIfStatementStore.end()) {
    usedConditionVariableToIfStatementStore[*conditionVariable] =
        std::make_shared<std::unordered_set<IfStatement *>>();
    return usedConditionVariableToIfStatementStore[*conditionVariable].get();
  }
  return usedConditionVariableToIfStatementStore[*conditionVariable].get();
}
