#pragma once
#include <vector>
#include "i_node.h"

namespace ast {
class StatementListNode : public INode {
public:
  void AddStatement(INode *node);

private:
  std::vector<INode *> statements;
};
}
