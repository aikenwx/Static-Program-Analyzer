#include "while_node.h"

namespace ast {
WhileNode::WhileNode(std::shared_ptr<INode> condition,
                     std::shared_ptr<StatementListNode> body) {
  this->condition = condition;
  this->body = body;
  IncrementStatementNumber(1);
}

std::shared_ptr<INode> WhileNode::GetCondition() { return condition; }

std::shared_ptr<StatementListNode> WhileNode::GetBody() { return body; }

std::string WhileNode::ToString() const {
  return "while:\n{\ncondition:" + condition->ToString() +
         "body:" + body->ToString() + "}\n";
}

int WhileNode::GetEndStatementNumber() { return body->GetEndStatementNumber(); }

void WhileNode::IncrementStatementNumber(int value) {
  statementNumber = body->GetStartStatementNumber();
  body->IncrementStatementNumbers(value);
}

void WhileNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);
  extractor->HandleWhileNode(std::static_pointer_cast<WhileNode>(currentNode),
                             depth);

  condition->AcceptVisitor(condition, extractor, depth + 1);
  body->AcceptVisitor(body, extractor, depth + 1);
}
}  // namespace ast
