#include "WhileStatement.h"


WhileStatement::WhileStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}

EntityType WhileStatement::getEntityType() {
    return EntityType::WHILE_STATEMENT;
}





