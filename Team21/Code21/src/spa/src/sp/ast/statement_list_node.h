#pragma once

#include <vector>
#include "i_node.h"

class StatementListNode : public INode {
public:
  void addStatement(INode *node);

private:
  std::vector<INode *> statements;
};