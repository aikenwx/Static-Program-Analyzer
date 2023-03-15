#include "AssignStatement.h"

#include <stdexcept>

AssignStatement::AssignStatement(int statementNumber) : Statement(statementNumber) {
}
EntityType &AssignStatement::getEntityType() const {
    return AssignStatement::assignStatementType;
}

EntityType &AssignStatement::getEntityTypeStatic() {
    return AssignStatement::assignStatementType;
}

void AssignStatement::setPostfixExpression(std::string *postfixExpression) {
    this->postFixExpression = std::shared_ptr<std::string>(postfixExpression);
}

std::string *AssignStatement::getPostFixExpression() {
    if (this->postFixExpression == nullptr) {
        throw std::runtime_error("Postfix expression is null, please check if it is set");
    }

    return this->postFixExpression.get();
}

EntityType AssignStatement::assignStatementType = EntityType();
