#pragma once

#include <memory>
#include <string>

#include "named_node.h"
#include "statement_list_node.h"

namespace ast {
class ProcedureNode : public NamedNode {
public:
  ProcedureNode(std::string name, std::shared_ptr<StatementListNode> statements);

  // Format is a stack
  auto GetStatements() -> std::shared_ptr<StatementListNode>;
  [[nodiscard]] auto ToString() const -> std::string override;
  auto GetStartStatementNumber() -> int;
  auto GetEndStatementNumber() -> int;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;

private:
  // Creates some coupling with statement list node
  std::shared_ptr<StatementListNode> statements;
};
}  // namespace ast
