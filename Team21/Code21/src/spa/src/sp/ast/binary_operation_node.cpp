#include "binary_operation_node.h"

namespace ast {
BinaryOperationNode::BinaryOperationNode(INode *left, INode *right) {
  this->left = left;
  this->right = right;
}

INode *BinaryOperationNode::GetLeft() {
  return left;
}

INode *BinaryOperationNode::GetRight() {
  return right;
}
}
