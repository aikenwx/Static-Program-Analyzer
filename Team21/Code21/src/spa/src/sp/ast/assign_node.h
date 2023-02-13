#pragma once
#include "expression_node.h"
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class AssignNode : public StatementNode {
public:
  AssignNode(VariableNode *var, ExpressionNode *exp);

  VariableNode *GetVariable();
  INode *GetAssignment();
  std::ostream &Write(std::ostream &out) const override;

private:
  VariableNode *var;
  INode *assignment
};
}
