#include "while_node.h"

#include <utility>

namespace ast {
WhileNode::WhileNode(std::shared_ptr<INode> condition,
                     std::shared_ptr<StatementListNode> body)
    : condition(std::move(std::move(condition))),
      body(std::move(std::move(body))) {
  IncrementStatementNumber(1);
}

auto WhileNode::GetCondition() -> std::shared_ptr<INode> { return condition; }

auto WhileNode::GetBody() -> std::shared_ptr<StatementListNode> { return body; }

auto WhileNode::ToString() const -> std::string {
  return "while:\n{\ncondition:" + condition->ToString() +
         "body:" + body->ToString() + "}\n";
}

auto WhileNode::GetEndStatementNumber() -> int {
  return body->GetEndStatementNumber();
}

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
