#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber) : Statement(&ReadStatement::getEntityTypeStatic(), statementNumber) {
}

EntityType &ReadStatement::getEntityTypeStatic() {
    return ReadStatement::readStatementType;
}

EntityType &ReadStatement::getEntityType() const {
    return ReadStatement::getEntityTypeStatic();
}

EntityType ReadStatement::readStatementType = EntityType();
