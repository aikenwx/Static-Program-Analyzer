#include "procedure_node.h"

#include <utility>

namespace ast {
ProcedureNode::ProcedureNode(std::string name,
                             std::shared_ptr<StatementListNode> statements)
    : NamedNode(std::move(name)), statements(std::move(statements)) {}

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

void ProcedureNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                  int depth) {
  extractor.HandleProcedureNode(
      std::static_pointer_cast<ProcedureNode>(shared_from_this()), depth);

  statements->AcceptVisitor(extractor, depth + 1);
}
} // namespace ast
