#pragma once
#include "conditional_node.h"
#include "statement_list_node.h"

namespace ast {
class IfNode : public ConditionalNode {
public:
  IfNode(std::shared_ptr<INode> condition,
         std::shared_ptr<StatementListNode> then,
         std::shared_ptr<StatementListNode> els);

  [[nodiscard]] auto GetCondition() const -> std::shared_ptr<INode> override;
  [[nodiscard]] auto GetThen() const -> std::shared_ptr<StatementListNode>;
  [[nodiscard]] auto GetElse() const -> std::shared_ptr<StatementListNode>;
  [[nodiscard]] auto ToString() const -> std::string override;
  [[nodiscard]] auto GetEndStatementNumber() const -> int override;
  void IncrementStatementNumber(int value) override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> then;
  std::shared_ptr<StatementListNode> els;
};
} // namespace ast
