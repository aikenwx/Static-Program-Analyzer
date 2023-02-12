#include "AssignStatement.h"
#include <stdexcept>

AssignStatement::AssignStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
    this->postFixExpression = std::shared_ptr<std::string>(postFixExpression);
}

EntityType AssignStatement::getEntityType() {
    return EntityType::ASSIGN_STATEMENT;
}

void AssignStatement::setPostfixExpression(std::string *postfixExpression) {
    this->postFixExpression = std::shared_ptr<std::string>(postfixExpression);
}

std::string * AssignStatement::getPostFixExpression() {
    if (this->postFixExpression == nullptr) {
        throw std::runtime_error("Postfix expression is null, please check if it is set");
    }

    return this->postFixExpression.get();
}
