#pragma once

#include <memory>

#include "unary_operation_node.h"

namespace ast {
class NotNode : public UnaryOperationNode {
public:
  using UnaryOperationNode::UnaryOperationNode;

  [[nodiscard]] auto ToString() const -> std::string override;
};
}
