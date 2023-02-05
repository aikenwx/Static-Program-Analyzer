#pragma once
#include "named_node.h"

// Not sure if this class is needed but guarantees correct type checking
class NameNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  void execute() override;
};
