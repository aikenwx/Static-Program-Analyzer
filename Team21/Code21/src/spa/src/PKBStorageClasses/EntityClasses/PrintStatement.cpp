#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) : Statement(&PrintStatement::getEntityTypeStatic(), statementNumber) {
}

EntityType &PrintStatement::getEntityTypeStatic() {
    return PrintStatement::printStatementType;
}

EntityType &PrintStatement::getEntityType() const {
    return PrintStatement::getEntityTypeStatic();
}

EntityType PrintStatement::printStatementType = EntityType();
