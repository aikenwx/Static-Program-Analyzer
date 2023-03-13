#include "WhileStatement.h"

EntityType &WhileStatement::getEntityType() const {
    return WhileStatement::whileStatementType;
}

WhileStatement::WhileStatement(int statementNumber) : Statement(statementNumber) {
}

EntityType &WhileStatement::getEntityTypeStatic() {
    return WhileStatement::whileStatementType;
}

EntityType WhileStatement::whileStatementType = EntityType();
