#pragma once
#include "container_statement_node.h"
#include "conditional_expression_node.h"
#include "sp/ast/conditional_node.h"
#include "statement_list_node.h"

namespace ast {
class WhileNode : public ConditionalNode {
public:
  WhileNode(std::shared_ptr<INode> condition, std::shared_ptr<StatementListNode> body);

  [[nodiscard]] auto GetCondition() const -> std::shared_ptr<INode> override;
  [[nodiscard]] auto GetBody() const -> std::shared_ptr<StatementListNode>;
  ;
  [[nodiscard]] auto ToString() const -> std::string override;
  [[nodiscard]] auto GetEndStatementNumber() const -> int override;
  void IncrementStatementNumber(int value) override;

  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> body;
};
}  // namespace ast
