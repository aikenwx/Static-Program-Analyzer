#include "binary_operation_node.h"

namespace ast {
BinaryOperationNode::BinaryOperationNode(std::shared_ptr<INode> left,
                                         std::shared_ptr<INode> right) {
  this->left = left;
  this->right = right;
}

std::shared_ptr<INode> BinaryOperationNode::GetLeft() { return left; }

std::shared_ptr<INode> BinaryOperationNode::GetRight() { return right; }

void BinaryOperationNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  // nb: no HandleBinaryOperationNode() in Extractor

  left->AcceptVisitor(left, extractor, depth + 1);
  right->AcceptVisitor(right, extractor, depth + 1);
}
}  // namespace ast
