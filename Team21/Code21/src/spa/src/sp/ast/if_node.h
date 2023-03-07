#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ContainerStatementNode {
public:
  IfNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> then, std::shared_ptr<StatementListNode> els);

  std::shared_ptr<INode> GetCondition();
  std::shared_ptr<StatementListNode> GetThen();
  std::shared_ptr<StatementListNode> GetElse();
  std::string ToString() const override;
  int GetEndStatementNumber() override;
  void IncrementStatementNumber(int value) override;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> then;
  std::shared_ptr<StatementListNode> els;
};
}
