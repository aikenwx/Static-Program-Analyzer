#include "AssignStatement.h"

#include <stdexcept>
#include <utility>

AssignStatement::AssignStatement(int statementNumber) : Statement(&AssignStatement::getEntityTypeStatic(), statementNumber) {
}
auto AssignStatement::getEntityType() const -> const EntityType & {
  return AssignStatement::assignStatementType;
}

auto AssignStatement::getEntityTypeStatic() -> const EntityType & {
  return AssignStatement::assignStatementType;
}

void AssignStatement::setPostfixExpression(std::shared_ptr<std::string> postfixExpression) {
    this->postFixExpression = std::move(postfixExpression);
}

auto AssignStatement::getPostFixExpression() -> std::string * {
  if (this->postFixExpression == nullptr) {
    throw std::runtime_error(
        "Postfix expression is null, please check if it is set");
  }

  return this->postFixExpression.get();
}

const EntityType AssignStatement::assignStatementType = EntityType();
