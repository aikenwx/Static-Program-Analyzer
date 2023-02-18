#include "binary_operation_node.h"

namespace ast {
BinaryOperationNode::BinaryOperationNode(std::shared_ptr<INode> left, std::shared_ptr<INode> right) {
  this->left = left;
  this->right = right;
}

std::shared_ptr<INode> BinaryOperationNode::GetLeft() {
  return left;
}

std::shared_ptr<INode> BinaryOperationNode::GetRight() {
  return right;
}
}
