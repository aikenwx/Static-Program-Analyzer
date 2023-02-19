//
// Created by Aiken Wong on 12/2/23.
//

#include "PatternManager.h"


void PatternManager::storeAssignStatementPostfixExpression(AssignStatement *assignStatement, std::string* postfixExpression) {
    assignStatement->setPostfixExpression(postfixExpression);
}
