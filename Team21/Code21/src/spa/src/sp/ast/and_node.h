#pragma once
#include "binary_operation_node.h"

namespace ast {
class AndNode : public BinaryOperationNode {
public:
  using BinaryOperationNode::BinaryOperationNode;

  [[nodiscard]] auto ToString() const -> std::string override;
};
}
