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

 public:
  PatternManager() = default;
  static void storeAssignStatementPostfixExpression(
          AssignStatement* assignStatement,
          std::unique_ptr<std::string> postfixExpression);

};

#endif  // SPA_PATTERNMANAGER_H
