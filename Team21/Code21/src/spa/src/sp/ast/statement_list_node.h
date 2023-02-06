#pragma once
#include <vector>
#include "i_node.h"

namespace ast {
class StatementListNode : public INode {
public:
  void AddStatement(INode *node);
  // Format is a stack
  std::vector<INode *> *GetStatements();
  std::ostream &Write(std::ostream &out) const override;

private:
  std::vector<INode *> statements;
};
}
