#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class WhileNode : public ContainerStatementNode {
public:
  WhileNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> body);

  std::shared_ptr<INode> GetCondition();
  std::shared_ptr<StatementListNode> GetBody();;
  std::string ToString() const override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> body;
};
}
