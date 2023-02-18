#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class WhileNode : public ContainerStatementNode {
public:
  WhileNode(INode *condition, StatementListNode *body);

  INode *GetCondition();
  StatementListNode *GetBody();;
  std::string ToString() const override;

private:
  INode *condition;
  StatementListNode *body;
};
}
