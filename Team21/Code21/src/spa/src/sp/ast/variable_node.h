#pragma once
#include "named_node.h"

namespace ast {
class VariableNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  [[nodiscard]] auto ToString() const -> std::string override;

  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;
};
}  // namespace ast
