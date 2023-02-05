#pragma once

#include "i_node.h"

class BinaryOperationNode : public INode {
private:
  INode left;
  INode right;
};