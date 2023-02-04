#include "CallStatement.h"


CallStatement::CallStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}


EntityType CallStatement::getEntityType() {
    return EntityType::CALL_STATEMENT;
}
