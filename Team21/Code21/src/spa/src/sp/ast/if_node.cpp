#include "if_node.h"

namespace ast {
IfNode::IfNode(std::shared_ptr<ConditionalExpressionNode> condition, std::shared_ptr<StatementListNode> then, std::shared_ptr<StatementListNode> els) {
  this->condition = condition;
  this->then = then;
  this->els = els;
}

std::shared_ptr<ConditionalExpressionNode> IfNode::GetCondition() {
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
}
