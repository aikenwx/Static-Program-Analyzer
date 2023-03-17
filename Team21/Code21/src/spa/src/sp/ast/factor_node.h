#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class FactorNode : public INode {
public:
 explicit FactorNode(std::shared_ptr<INode> operand);

 auto GetOperand() -> std::shared_ptr<INode>;
 [[nodiscard]] auto ToString() const -> std::string override;

private:
  std::shared_ptr<INode> operand;
};
}  // namespace ast
