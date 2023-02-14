#pragma once
#include "i_node.h"

namespace ast {
class ConstantNode : public INode {
public:
  ConstantNode(int value);

  void Execute();
  int GetValue();
  std::string ToString() const override;

private:
  int value;
};
}
