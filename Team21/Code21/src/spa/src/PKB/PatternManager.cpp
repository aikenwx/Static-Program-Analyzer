//
// Created by Aiken Wong on 12/2/23.
//

#include "PatternManager.h"

#include <utility>

void PatternManager::storeAssignStatementPostfixExpression(
        AssignStatement *assignStatement,
        std::unique_ptr<std::string> postfixExpression) {
  assignStatement->setPostfixExpression(std::move(postfixExpression));
}

void PatternManager::storeWhileStatementConditionVariable(
    WhileStatement *whileStatement, Variable *conditionVariable) {
  usedConditionVariableToWhileStatementStore.try_emplace(
      *conditionVariable->getEntityValue(),
      std::make_shared<std::unordered_set<WhileStatement *>>());
  usedConditionVariableToWhileStatementStore[*conditionVariable
                                                  ->getEntityValue()]
      ->insert(whileStatement);

  whileStatementToUsedConditionVariablesStore.try_emplace(
      whileStatement->getStatementNumber(),
      std::make_shared<std::unordered_set<Variable *>>());
  whileStatementToUsedConditionVariablesStore[whileStatement
                                                  ->getStatementNumber()]
      ->insert(conditionVariable);
}

void PatternManager::storeIfStatementConditionVariable(
    IfStatement *ifStatement, Variable *conditionVariable) {
  usedConditionVariableToIfStatementStore.try_emplace(
      *conditionVariable->getEntityValue(),
      std::make_shared<std::unordered_set<IfStatement *>>());
  usedConditionVariableToIfStatementStore[*conditionVariable->getEntityValue()]
      ->insert(ifStatement);

  ifStatementToUsedConditionVariablesStore.try_emplace(
      ifStatement->getStatementNumber(),
      std::make_shared<std::unordered_set<Variable *>>());
  ifStatementToUsedConditionVariablesStore[ifStatement->getStatementNumber()]
      ->insert(conditionVariable);
}

auto PatternManager::getWhileStatementsByConditionVariable(
    std::string *conditionVariable) -> std::unordered_set<WhileStatement *> * {
  usedConditionVariableToWhileStatementStore.try_emplace(
      *conditionVariable,
      std::make_shared<std::unordered_set<WhileStatement *>>());
  return usedConditionVariableToWhileStatementStore[*conditionVariable].get();
}

auto PatternManager::getIfStatementsByConditionVariable(
    std::string *conditionVariable) -> std::unordered_set<IfStatement *> * {
  usedConditionVariableToIfStatementStore.try_emplace(
      *conditionVariable,
      std::make_shared<std::unordered_set<IfStatement *>>());
  return usedConditionVariableToIfStatementStore[*conditionVariable].get();
}

auto PatternManager::getVariablesUsedInWhileStatementCondition(
    int whileStatementNumber) -> std::unordered_set<Variable *> * {
  whileStatementToUsedConditionVariablesStore.try_emplace(
      whileStatementNumber, std::make_shared<std::unordered_set<Variable *>>());
  return whileStatementToUsedConditionVariablesStore[whileStatementNumber]
      .get();
}

auto PatternManager::getVariablesUsedInIfStatementCondition(
    int ifStatementNumber) -> std::unordered_set<Variable *> * {
  ifStatementToUsedConditionVariablesStore.try_emplace(
      ifStatementNumber, std::make_shared<std::unordered_set<Variable *>>());
  return ifStatementToUsedConditionVariablesStore[ifStatementNumber].get();
}
