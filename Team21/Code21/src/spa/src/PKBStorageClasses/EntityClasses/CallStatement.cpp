#include "CallStatement.h"

CallStatement::CallStatement(int statementNumber) : Statement(&CallStatement::getEntityTypeStatic(), statementNumber) {
}

EntityType& CallStatement::getEntityTypeStatic() {
    return CallStatement::callStatementType;
}

EntityType& CallStatement::getEntityType() const {
    return CallStatement::callStatementType;
}

EntityType CallStatement::callStatementType = EntityType();
