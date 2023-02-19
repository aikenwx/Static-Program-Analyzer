#pragma once

#include <memory>

#include "unary_operation_node.h"

namespace ast {
class NotNode : public UnaryOperationNode {
public:
  using UnaryOperationNode::UnaryOperationNode;

  std::string ToString() const override;
};
}
