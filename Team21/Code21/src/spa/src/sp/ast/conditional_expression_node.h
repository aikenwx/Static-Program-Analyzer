#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class ConditionalExpressionNode : public INode {
public:
  ConditionalExpressionNode(std::shared_ptr<INode> operand);

  std::shared_ptr<INode> GetOperand();
  std::string ToString() const override;

private:
  std::shared_ptr<INode> operand;
};
}
