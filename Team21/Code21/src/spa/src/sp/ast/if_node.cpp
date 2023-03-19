#include "if_node.h"

#include <utility>

namespace ast {
IfNode::IfNode(std::shared_ptr<INode> condition,
               std::shared_ptr<StatementListNode> then,
               std::shared_ptr<StatementListNode> els)
    : condition(std::move(condition)),
      then(std::move(then)),
      els(std::move(els)) {
  IncrementStatementNumber(1);
}

auto IfNode::GetCondition() const -> std::shared_ptr<INode> { return condition; }

auto IfNode::GetThen() const -> std::shared_ptr<StatementListNode> { return then; }

auto IfNode::GetElse() const -> std::shared_ptr<StatementListNode> { return els; }

auto IfNode::ToString() const -> std::string {
  return "if:\n{\ncondition:" + condition->ToString() +
         "then:" + then->ToString() + "else:" + els->ToString() + "}\n";
}

auto IfNode::GetEndStatementNumber() const -> int {
  return els->GetEndStatementNumber();
}

void IfNode::IncrementStatementNumber(int value) {
  SetStatementNumber(then->GetStartStatementNumber());
  then->IncrementStatementNumbers(value);
  els->IncrementStatementNumbers(value);
}

void IfNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleConditionalNode(std::static_pointer_cast<ConditionalNode>(currentNode),
                                   depth);
  extractor->HandleIfNode(std::static_pointer_cast<IfNode>(currentNode), depth);

  condition->AcceptVisitor(condition, extractor, depth + 1);
  then->AcceptVisitor(then, extractor, depth + 1);
  els->AcceptVisitor(els, extractor, depth + 1);
}
}  // namespace ast
