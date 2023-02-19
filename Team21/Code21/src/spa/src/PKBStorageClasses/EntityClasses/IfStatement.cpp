#include "IfStatement.h"

IfStatement::IfStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
}

EntityType IfStatement::getEntityType() {
    return EntityType::IF_STATEMENT;
}
