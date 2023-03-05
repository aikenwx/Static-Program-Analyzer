#include "if_node.h"

namespace ast {
IfNode::IfNode(std::shared_ptr<INode> condition,
               std::shared_ptr<StatementListNode> then,
               std::shared_ptr<StatementListNode> els) {
  this->condition = condition;
  this->then = then;
  this->els = els;
  IncrementStatementNumber(1);
}

std::shared_ptr<INode> IfNode::GetCondition() { return condition; }

std::shared_ptr<StatementListNode> IfNode::GetThen() { return then; }

std::shared_ptr<StatementListNode> IfNode::GetElse() { return els; }

std::string IfNode::ToString() const {
  return "if:\n{\ncondition:" + condition->ToString() +
         "then:" + then->ToString() + "else:" + els->ToString() + "}\n";
}

int IfNode::GetEndStatementNumber() { return els->GetEndStatementNumber(); }

void IfNode::IncrementStatementNumber(int value) {
  statementNumber = then->GetStartStatementNumber();
  then->IncrementStatementNumbers(value);
  els->IncrementStatementNumbers(value);
}

void IfNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleIfNode(std::static_pointer_cast<IfNode>(currentNode), depth);

  condition->AcceptVisitor(condition, extractor, depth + 1);
  then->AcceptVisitor(then, extractor, depth + 1);
  els->AcceptVisitor(els, extractor, depth + 1);
}
}  // namespace ast
