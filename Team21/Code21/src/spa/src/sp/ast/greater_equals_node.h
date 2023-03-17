#pragma once
#include "binary_operation_node.h"

namespace ast {
class GreaterEqualsNode : public BinaryOperationNode {
public:
  using BinaryOperationNode::BinaryOperationNode;

  [[nodiscard]] auto ToString() const -> std::string override;
};
}
