#pragma once
#include "named_node.h"

namespace ast {
class VariableNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  std::string ToString() const override;
};
}
