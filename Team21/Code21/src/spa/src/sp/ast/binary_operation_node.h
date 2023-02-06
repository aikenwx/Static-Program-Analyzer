#pragma once
#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
private:
  INode *left;
  INode *right;
};
}
