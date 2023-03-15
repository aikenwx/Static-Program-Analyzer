#include "variable_node.h"

namespace ast {
auto VariableNode::ToString() const -> std::string {
  return "variable:" + NamedNode::ToString();
}

void VariableNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  extractor->HandleVariableNode(
      std::static_pointer_cast<VariableNode>(currentNode), depth);
}
}  // namespace ast
