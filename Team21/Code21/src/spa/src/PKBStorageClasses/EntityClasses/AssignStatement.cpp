#include "AssignStatement.h"

AssignStatement::AssignStatement(int statementNumber, std::string* postFixExpression) {
    Statement::statementNumber = statementNumber;
    Statement::statementNumberString = std::make_shared<std::string>(std::to_string(statementNumber));
    this->postFixExpression = std::shared_ptr<std::string>(postFixExpression);
}

EntityType AssignStatement::getEntityType() {
    return EntityType::ASSIGN_STATEMENT;
}

std::string * AssignStatement::getPostFixExpression() {
    return this->postFixExpression.get();
}
