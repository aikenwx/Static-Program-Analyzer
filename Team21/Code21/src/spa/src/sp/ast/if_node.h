#pragma once
#include "sp/ast/conditional_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ConditionalNode {
public:
  IfNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> then, std::shared_ptr<StatementListNode> els);

  auto GetCondition() -> std::shared_ptr<INode> override;
  auto GetThen() -> std::shared_ptr<StatementListNode>;
  auto GetElse() -> std::shared_ptr<StatementListNode>;
  [[nodiscard]] auto ToString() const -> std::string override;
  auto GetEndStatementNumber() -> int override;
  void IncrementStatementNumber(int value) override;

  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> then;
  std::shared_ptr<StatementListNode> els;
};
}  // namespace ast
