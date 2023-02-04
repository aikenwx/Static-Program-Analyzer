#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}

EntityType PrintStatement::getEntityType() {
    return EntityType::PRINT_STATEMENT;
}





