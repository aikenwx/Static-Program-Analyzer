#pragma once
#include "named_node.h"

namespace ast {
class VariableNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  //void Execute() override;
  std::ostream &Write(std::ostream &out) const override;
};
}
