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
#include "../PKBStorageClasses/EntityClasses/Variable.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"

class PatternManager {
 private:
  std::unordered_map<std::string,
                     std::shared_ptr<std::unordered_set<WhileStatement*>>>
      usedConditionVariableToWhileStatementStore;

  std::unordered_map<std::string,
                     std::shared_ptr<std::unordered_set<IfStatement*>>>
      usedConditionVariableToIfStatementStore;

  std::unordered_map<int, std::shared_ptr<std::unordered_set<Variable*>>>
      whileStatementToUsedConditionVariablesStore;

  std::unordered_map<int, std::shared_ptr<std::unordered_set<Variable*>>>
      ifStatementToUsedConditionVariablesStore;

 public:
  PatternManager() = default;
  static void storeAssignStatementPostfixExpression(
          AssignStatement* assignStatement,
          std::unique_ptr<std::string> postfixExpression);

  void storeWhileStatementConditionVariable(WhileStatement* whileStatement,
                                            Variable* conditionVariable);

  void storeIfStatementConditionVariable(IfStatement* ifStatement,
                                         Variable* conditionVariable);

  auto getWhileStatementsByConditionVariable(std::string* conditionVariable)
      -> std::unordered_set<WhileStatement*>*;

  auto getIfStatementsByConditionVariable(std::string* conditionVariable)
      -> std::unordered_set<IfStatement*>*;

  auto getVariablesUsedInWhileStatementCondition(int whileStatementNumber)
      -> std::unordered_set<Variable*>*;

  auto getVariablesUsedInIfStatementCondition(int ifStatementNumber)
      -> std::unordered_set<Variable*>*;
};

#endif  // SPA_PATTERNMANAGER_H
