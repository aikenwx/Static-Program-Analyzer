#include "if_node.h"

namespace ast {
IfNode::IfNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> then, std::shared_ptr<StatementListNode> els) {
  this->condition = condition;
  this->then = then;
  this->els = els;
  this->statementNumber = this->then->GetStartStatementNumber();
  this->then->IncrementStatementNumbers(1);
  this->els->IncrementStatementNumbers(1);
}

std::shared_ptr<INode> IfNode::GetCondition() {
  return condition;
}

std::shared_ptr<StatementListNode> IfNode::GetThen() {
  return then;
}

std::shared_ptr<StatementListNode> IfNode::GetElse() {
  return els;
}

std::string IfNode::ToString() const {
  return "if:\n{\ncondition:" + condition->ToString() + "then:" + then->ToString() + "else:" + els->ToString() + "}\n";
}

int IfNode::GetEndStatementNumber() {
  return els->GetEndStatementNumber();
}
}
