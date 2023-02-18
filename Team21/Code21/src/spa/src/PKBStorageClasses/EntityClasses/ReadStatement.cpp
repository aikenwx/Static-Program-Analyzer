#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
}

EntityType ReadStatement::getEntityType() {
    return EntityType::READ_STATEMENT;
}
