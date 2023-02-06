#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber)  {
    Statement::statementNumber = statementNumber;
}

EntityType ReadStatement::getEntityType() {
    return EntityType::READ_STATEMENT;
}

