#include "CallStatement.h"

CallStatement::CallStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
}

EntityType CallStatement::getEntityType() {
    return EntityType::CALL_STATEMENT;
}
