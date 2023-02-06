#pragma once
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class PrintNode : public StatementNode {
public:
  PrintNode(VariableNode *var);

  VariableNode *GetVariable();
  std::ostream &Write(std::ostream &out) const override;

private:
  VariableNode *var;
};
}
