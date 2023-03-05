#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) { this->value = value; }

int ConstantNode::GetValue() { return this->value; }

std::string ConstantNode::ToString() const {
  return "constant:" + std::to_string(value) + "\n";
}

void ConstantNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleConstantNode(
      std::static_pointer_cast<ConstantNode>(currentNode), depth);
}
}  // namespace ast
