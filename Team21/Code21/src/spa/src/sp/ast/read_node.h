#pragma once
#include "statement_node.h"
#include "variable_node.h"

class ReadNode : public StatementNode {
public:
  ReadNode(VariableNode *var);

  VariableNode *getVariable();

private:
  VariableNode *var;
};
