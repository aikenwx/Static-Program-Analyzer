#pragma once
#include "binary_operation_node.h"
#include "sp/ast/symbol_node.h"

namespace ast {
class ComparisonOperationNode : public BinaryOperationNode {
public:
  explicit ComparisonOperationNode(std::shared_ptr<INode> left,
                      std::shared_ptr<INode> right,
                      SymbolType type);

  [[nodiscard]] auto GetSymbolType() const -> std::string;
  [[nodiscard]] auto ToString() const -> std::string override;

private:
  std::string operator_;
};
}  // namespace ast
