#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) : value(value) {}

auto ConstantNode::GetValue() const -> int { return this->value; }

auto ConstantNode::ToString() const -> std::string {
  return "constant:" + std::to_string(value) + "\n";
}

void ConstantNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  extractor->HandleConstantNode(
      std::static_pointer_cast<ConstantNode>(currentNode), depth);
}
}  // namespace ast
