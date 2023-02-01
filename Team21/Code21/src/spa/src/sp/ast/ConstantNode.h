#pragma once

#include "INode.h"

class ConstantNode : public INode {
private:
  int value;
public:
  ConstantNode(int value);

  void execute();
  int getValue();
};