#include "WhileStatement.h"

WhileStatement::WhileStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
}

EntityType WhileStatement::getEntityType() {
    return EntityType::WHILE_STATEMENT;
}
