#pragma once
#include "named_node.h"

class VariableNode : public NamedNode {
public:
  using NamedNode::NamedNode;

  void execute() override;
};