#include "while_node.h"
#include "sp/ast/conditional_node.h"

#include <memory>
#include <utility>

namespace ast {
WhileNode::WhileNode(std::shared_ptr<INode> condition,
                     std::shared_ptr<StatementListNode> body)
    : condition(std::move(condition)), body(std::move(body)) {
  IncrementStatementNumber(1);
}

auto WhileNode::GetCondition() const -> std::shared_ptr<INode> { return condition; }

auto WhileNode::GetBody() const -> std::shared_ptr<StatementListNode> { return body; }

auto WhileNode::ToString() const -> std::string {
  return "while:\n{\ncondition:" + condition->ToString() +
         "body:" + body->ToString() + "}\n";
}

auto WhileNode::GetEndStatementNumber() const -> int {
  return body->GetEndStatementNumber();
}

void WhileNode::IncrementStatementNumber(int value) {
  SetStatementNumber(body->GetStartStatementNumber());
  body->IncrementStatementNumbers(value);
}

void WhileNode::AcceptVisitor(design_extractor::Extractor &extractor,
                              int depth) {
  auto currentNode = shared_from_this();
  extractor.HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);
  extractor.HandleConditionalNode(
      std::static_pointer_cast<ConditionalNode>(currentNode), depth);
  extractor.HandleWhileNode(std::static_pointer_cast<WhileNode>(currentNode),
                             depth);

  condition->AcceptVisitor(extractor, depth + 1);
  body->AcceptVisitor(extractor, depth + 1);
}
} // namespace ast
