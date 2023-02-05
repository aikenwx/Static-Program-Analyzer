#pragma once

#include "i_node.h"

class AST {
public:
  INode GetRoot();

private:
  INode root;
};