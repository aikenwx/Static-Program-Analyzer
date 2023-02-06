#include "IfStatement.h"


IfStatement::IfStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}

EntityType IfStatement::getEntityType() {
    return EntityType::IF_STATEMENT;
}





