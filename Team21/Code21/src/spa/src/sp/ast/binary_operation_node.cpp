#include "binary_operation_node.h"

#include <utility>

namespace ast {
BinaryOperationNode::BinaryOperationNode(std::shared_ptr<INode> left,
                                         std::shared_ptr<INode> right)
    : left(std::move(std::move(left))), right(std::move(std::move(right))) {}

auto BinaryOperationNode::GetLeft() -> std::shared_ptr<INode> { return left; }

auto BinaryOperationNode::GetRight() -> std::shared_ptr<INode> { return right; }

void BinaryOperationNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  // nb: no HandleBinaryOperationNode() in Extractor
  // call HandleOtherNode() to let extractor update state
  // e.g. node parents
  extractor->HandleOtherNode(currentNode, depth);

  left->AcceptVisitor(left, extractor, depth + 1);
  right->AcceptVisitor(right, extractor, depth + 1);
}
}  // namespace ast
