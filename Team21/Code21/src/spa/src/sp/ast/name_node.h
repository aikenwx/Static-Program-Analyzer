#pragma once
#include "named_node.h"

namespace ast {
// Not sure if this class is needed but guarantees correct type checking
class NameNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  [[nodiscard]] auto ToString() const -> std::string override;
};
}
