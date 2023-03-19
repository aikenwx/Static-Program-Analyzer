#include "constant_node.h"

namespace ast {
ConstantNode::ConstantNode(int value) : value(value) {}

auto ConstantNode::GetValue() const -> int { return this->value; }

auto ConstantNode::ToString() const -> std::string {
  return "constant:" + std::to_string(value) + "\n";
}

void ConstantNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                 int depth) {
  extractor.HandleConstantNode(
      std::static_pointer_cast<ConstantNode>(shared_from_this()), depth);
}
} // namespace ast
