#pragma once

#include <memory>

#include "expression_node.h"
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class AssignNode : public StatementNode {
 public:
  AssignNode(std::shared_ptr<VariableNode> var,
             const std::shared_ptr<ExpressionNode>& exp);

  auto GetVariable() -> std::shared_ptr<VariableNode>;
  auto GetAssignment() -> std::shared_ptr<INode>;
  [[nodiscard]] auto ToString() const -> std::string override;
  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

 private:
  std::shared_ptr<VariableNode> var;
  std::shared_ptr<INode> assignment;
};
}  // namespace ast
