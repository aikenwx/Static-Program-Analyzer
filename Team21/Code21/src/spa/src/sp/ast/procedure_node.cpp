#include "procedure_node.h"

#include <utility>

namespace ast {
ProcedureNode::ProcedureNode(std::string name,
                             std::shared_ptr<StatementListNode> statements)
    : NamedNode(std::move(name)),
      statements(std::move(std::move(statements))) {}

auto ProcedureNode::GetStatements() -> std::shared_ptr<StatementListNode> {
  return statements;
}

auto ProcedureNode::ToString() const -> std::string {
  return "procedure:\n{\n" + statements->ToString() + "}\n";
}

auto ProcedureNode::GetStartStatementNumber() -> int {
  return statements->GetStartStatementNumber();
}

auto ProcedureNode::GetEndStatementNumber() -> int {
  return statements->GetEndStatementNumber();
}

void ProcedureNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleProcedureNode(
      std::static_pointer_cast<ProcedureNode>(currentNode), depth);

  statements->AcceptVisitor(statements, extractor, depth + 1);
}
}  // namespace ast
