#pragma once
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class ReadNode : public StatementNode {
public:
  ReadNode(VariableNode *var);

  VariableNode *GetVariable();
  std::string ToString() const override;

private:
  VariableNode *var;
};
}
