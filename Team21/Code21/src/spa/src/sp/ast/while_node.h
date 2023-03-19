#pragma once
#include "conditional_expression_node.h"
#include "container_statement_node.h"
#include "statement_list_node.h"

namespace ast {
class WhileNode : public ContainerStatementNode {
public:
  WhileNode(std::shared_ptr<INode> condition,
            std::shared_ptr<StatementListNode> body);

  auto GetCondition() -> std::shared_ptr<INode>;
  auto GetBody() -> std::shared_ptr<StatementListNode>;
  ;
  [[nodiscard]] auto ToString() const -> std::string override;
  auto GetEndStatementNumber() -> int override;
  void IncrementStatementNumber(int value) override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> condition;
  std::shared_ptr<StatementListNode> body;
};
} // namespace ast
