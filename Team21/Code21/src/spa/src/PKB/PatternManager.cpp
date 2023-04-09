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
