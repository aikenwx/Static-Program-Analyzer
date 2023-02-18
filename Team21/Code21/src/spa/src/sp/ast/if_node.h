#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ContainerStatementNode {
public:
  IfNode(ConditionalExpressionNode *condition, StatementListNode *then, StatementListNode *els);

  ConditionalExpressionNode *GetCondition();
  StatementListNode *GetThen();
  StatementListNode *GetElse();
  std::string ToString() const override;

private:
  ConditionalExpressionNode *condition;
  StatementListNode *then;
  StatementListNode *els;
};
}
