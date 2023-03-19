#pragma once
#include "i_node.h"

namespace ast {
class StatementNode : public INode {
 public:
  [[nodiscard]] auto GetStatementNumber() const -> int;
  void SetStatementNumber(int statementNumber);
  virtual void IncrementStatementNumber(int value);
  [[nodiscard]] auto ToString() const -> std::string override = 0;

  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

 private:
  int statementNumber;
};
}  // namespace ast
