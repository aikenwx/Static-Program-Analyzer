#pragma once
#include "binary_operation_node.h"

namespace ast {
class MinusNode : public BinaryOperationNode {
public:
  using BinaryOperationNode::BinaryOperationNode;

  std::string ToString() const override;
};
}
