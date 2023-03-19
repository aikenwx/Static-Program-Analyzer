#pragma once
#include <memory>
#include <vector>

#include "statement_node.h"

namespace ast {
class StatementListNode : public INode {
 public:
  void AddStatement(const std::shared_ptr<StatementNode>& node);
  // Format is a stack, i.e. statements are stored in reverse order
  [[nodiscard]] auto GetStatements() const
      -> std::vector<std::shared_ptr<StatementNode>>;
  [[nodiscard]] auto ToString() const -> std::string override;
  [[nodiscard]] auto GetStartStatementNumber() const -> int;
  [[nodiscard]] auto GetEndStatementNumber() const -> int;
  void IncrementStatementNumbers(int value);

  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

 private:
  std::vector<std::shared_ptr<StatementNode>> statements;
  int startStatementNumber;
  int endStatementNumber;
};
}  // namespace ast
