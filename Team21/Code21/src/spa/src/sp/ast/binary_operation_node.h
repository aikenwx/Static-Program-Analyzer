#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
public:
  BinaryOperationNode(std::shared_ptr<INode> left, std::shared_ptr<INode> right);

  std::shared_ptr<INode> GetLeft();
  std::shared_ptr<INode> GetRight();
  virtual std::string ToString() const override = 0;

protected:
  std::shared_ptr<INode> left;
  std::shared_ptr<INode> right;
};
}
