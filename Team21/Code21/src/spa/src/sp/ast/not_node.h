#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class NotNode : public INode {
public:
  NotNode(std::shared_ptr<INode> operand);

  std::shared_ptr<INode> GetOperand();
  std::string ToString() const override;

private:
  std::shared_ptr<INode> operand;
};
}
