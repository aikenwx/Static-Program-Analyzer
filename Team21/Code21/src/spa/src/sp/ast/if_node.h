#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ContainerStatementNode {
public:
  IfNode(INode *condition, StatementListNode *then, StatementListNode *els);

  INode *GetCondition();
  StatementListNode *GetThen();
  StatementListNode *GetElse();
  std::string ToString() const override;

private:
  INode *condition;
  StatementListNode *then;
  StatementListNode *els;
};
}
