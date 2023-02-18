#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class WhileNode : public ContainerStatementNode {
public:
  WhileNode(ConditionalExpressionNode *condition, StatementListNode *body);

  ConditionalExpressionNode *GetCondition();
  StatementListNode *GetBody();;
  std::string ToString() const override;

private:
  ConditionalExpressionNode *condition;
  StatementListNode *body;
};
}
