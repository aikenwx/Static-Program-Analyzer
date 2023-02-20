#pragma once
#include "binary_operation_node.h"

namespace ast {
class DivideNode : public BinaryOperationNode {
public:
  using BinaryOperationNode::BinaryOperationNode;

  std::string ToString() const override;
};
}
