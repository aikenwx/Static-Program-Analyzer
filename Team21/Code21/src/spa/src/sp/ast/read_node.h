#pragma once
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class ReadNode : public StatementNode {
public:
  ReadNode(VariableNode *var);

  VariableNode *GetVariable();

private:
  VariableNode *var;
};
}
