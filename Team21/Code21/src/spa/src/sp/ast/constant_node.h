#pragma once

#include "i_node.h"

class ConstantNode : public INode {
public:
  ConstantNode(int value);

  void execute();
  int getValue();

private:
  int value;
};