#pragma once
#include "i_node.h"

namespace ast {
class ConstantNode : public INode {
public:
  ConstantNode(int value);

  void execute();
  int getValue();

private:
  int value;
};
}
