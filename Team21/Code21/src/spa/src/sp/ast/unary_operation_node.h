#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class UnaryOperationNode : public INode {
public:
  UnaryOperationNode(std::shared_ptr<INode> operand);

  std::shared_ptr<INode> GetOperand();
  virtual std::string ToString() const override = 0;

protected:
  std::shared_ptr<INode> operand;
};
}
