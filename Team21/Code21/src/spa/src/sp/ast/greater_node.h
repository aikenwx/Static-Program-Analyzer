#pragma once
#include "binary_operation_node.h"

namespace ast {
class GreaterNode : public BinaryOperationNode {
public:
  using BinaryOperationNode::BinaryOperationNode;

  std::string ToString() const override;
};
}
