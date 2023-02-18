#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
}

EntityType PrintStatement::getEntityType() {
    return EntityType::PRINT_STATEMENT;
}
