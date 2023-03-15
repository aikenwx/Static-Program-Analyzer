#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber) : Statement(statementNumber) {
}

EntityType &ReadStatement::getEntityTypeStatic() {
    return ReadStatement::readStatementType;
}

EntityType &ReadStatement::getEntityType() const {
    return ReadStatement::getEntityTypeStatic();
}

EntityType ReadStatement::readStatementType = EntityType();
