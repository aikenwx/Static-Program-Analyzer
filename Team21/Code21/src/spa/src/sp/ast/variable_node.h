#pragma once
#include "named_node.h"

namespace ast {
class VariableNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  [[nodiscard]] auto ToString() const -> std::string override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;
};
} // namespace ast
