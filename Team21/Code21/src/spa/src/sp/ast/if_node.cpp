#include "if_node.h"

namespace ast {
IfNode::IfNode(ConditionalExpressionNode *condition, StatementListNode *then, StatementListNode *els) {
  this->condition = condition;
  this->then = then;
  this->els = els;
}

ConditionalExpressionNode *IfNode::GetCondition() {
  return condition;
}

StatementListNode *IfNode::GetThen() {
  return then;
}

StatementListNode *IfNode::GetElse() {
  return els;
}

std::string IfNode::ToString() const {
  return "if:\n{\ncondition:" + condition->ToString() + "then:" + then->ToString() + "else:" + els->ToString() + "}\n";
}
}
