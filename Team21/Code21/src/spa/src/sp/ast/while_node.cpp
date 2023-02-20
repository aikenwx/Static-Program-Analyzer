#include "while_node.h"

namespace ast {
WhileNode::WhileNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> body) {
  this->condition = condition;
  this->body = body;
  IncrementStatementNumber(1);
}

std::shared_ptr<INode> WhileNode::GetCondition() {
  return condition;
}

std::shared_ptr<StatementListNode> WhileNode::GetBody() {
  return body;
}

std::string WhileNode::ToString() const {
  return "while:\n{\ncondition:" + condition->ToString() + "body:" + body->ToString() + "}\n";
}

int WhileNode::GetEndStatementNumber() {
  return body->GetEndStatementNumber();
}

void WhileNode::IncrementStatementNumber(int value) {
  statementNumber = body->GetStartStatementNumber();
  body->IncrementStatementNumbers(value);
}
}
