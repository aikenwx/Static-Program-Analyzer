#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ContainerStatementNode {
public:
  IfNode(std::shared_ptr<ConditionalExpressionNode> condition, std::shared_ptr<StatementListNode> then, std::shared_ptr<StatementListNode> els);

  std::shared_ptr<ConditionalExpressionNode> GetCondition();
  std::shared_ptr<StatementListNode> GetThen();
  std::shared_ptr<StatementListNode> GetElse();
  std::string ToString() const override;

private:
  std::shared_ptr<ConditionalExpressionNode> condition;
  std::shared_ptr<StatementListNode> then;
  std::shared_ptr<StatementListNode> els;
};
}
