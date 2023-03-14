#include "IfStatement.h"

EntityType IfStatement::ifStatementType = EntityType();

EntityType& IfStatement::getEntityTypeStatic() {
    return IfStatement::ifStatementType;
}

EntityType& IfStatement::getEntityType() const {
    return IfStatement::getEntityTypeStatic();
}

IfStatement::IfStatement(int statementNumber) : Statement(&IfStatement::getEntityTypeStatic(), statementNumber) {
}
