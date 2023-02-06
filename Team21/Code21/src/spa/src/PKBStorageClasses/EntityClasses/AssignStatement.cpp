#include "AssignStatement.h"


AssignStatement::AssignStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}

EntityType AssignStatement::getEntityType() {
    return EntityType::ASSIGN_STATEMENT;
}



